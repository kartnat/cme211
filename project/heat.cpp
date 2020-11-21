#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>

#include "CGSolver.hpp"
#include "heat.hpp"
#include "sparse.hpp"


  SparseMatrix A;
  std::vector<double> b, x;
  unsigned int nrows, ncols;
 
  int HeatEquation2D::Setup(std::string inputfile) {
   //Reading file attributes (length, width, height, etc.)
   std::ifstream f;
   f.open(inputfile.c_str());
   double length, width, height;
   double Tc, Th;
   if (f.is_open()) {
     f >> length >> width >> height;
     f >> Tc >> Th;
   }
   
   nrows = (unsigned int)(width/height + 1-2);
   ncols = (unsigned int)(length/height);
   double div = 1/(height*height); //delta h
   
   int k = 0;
   //Adding entries to matrix in COO format
   //Going by column so that the resultant matrix is symmetrix
   for (unsigned int col=0;col<ncols;col++) {
     for (unsigned int row=0;row<nrows;row++) {
       //Main cases are whether we are at a boundary or not
       if (row>= 1 && row<nrows-1 && col>=1 && col < ncols-1) {
         A.AddEntry(k, (col-1)*nrows+row,-div);
         A.AddEntry(k, (col+1)*nrows+row,-div);
         A.AddEntry(k, col*nrows+row,4*div);
         A.AddEntry(k, col*nrows+row-1,-div);
         A.AddEntry(k, col*nrows+row+1,-div);
         b.push_back(0); }
       //At a periodic boundary
       else if (row>= 1 && row<nrows-1 && (col==0 || col == ncols-1)) {
         if (col == 0) {
           A.AddEntry(k, row-1,-div);
           A.AddEntry(k, row,4*div);
           A.AddEntry(k,row+1,-div);
           A.AddEntry(k, nrows+row,-div);
           A.AddEntry(k,(ncols-1)*nrows+row,-div);
           b.push_back(0); }
         if (col == ncols-1) {
           A.AddEntry(k,col*nrows+row-1,-div);
           A.AddEntry(k,col*nrows+row,4*div);
           A.AddEntry(k,col*nrows+row+1,-div);
           A.AddEntry(k,(col-1)*nrows+row,-div);
           A.AddEntry(k,row,-div);
           b.push_back(0); }
         }
       //Isothermal boundary
       else if ((row==0 ||  row ==nrows-1) && col>=1 && col < ncols-1) {
         if (row==0) {
           A.AddEntry(k,(col-1)*nrows,-div);
           A.AddEntry(k,(col+1)*nrows,-div);
           A.AddEntry(k,col*nrows+1,-div);
           A.AddEntry(k,col*nrows,4*div);
           b.push_back(div*Th); }
         if (row == nrows-1) {
           A.AddEntry(k,(col-1)*nrows+row,-div);
           A.AddEntry(k,col*nrows+row-1,-div);
           A.AddEntry(k,(col+1)*nrows+row,-div);
           A.AddEntry(k,col*nrows+row,4*div);
           b.push_back(div*-Tc*(exp(-10*(col*height-length/2)*(col*height-length/2))-2)); }
       }
       //Corners (Isotermal and Periodic)
       else if (row==0 && col == 0) {
         A.AddEntry(k,(ncols-1)*nrows,-div);
         A.AddEntry(k,nrows,-div);
         A.AddEntry(k,1,-div);
         A.AddEntry(k,0,4*div);
         b.push_back(div*Th); }
         
       else if (row==0 && col==ncols-1) {
         A.AddEntry(k,(col-1)*nrows,-div);
         A.AddEntry(k,0,-div);
         A.AddEntry(k,col*nrows+1,-div);
         A.AddEntry(k,col*nrows,4*div);
         b.push_back(div*Th); }
         
       else if (row==nrows-1 && col==ncols-1) {
         A.AddEntry(k,(col-1)*nrows+row,-div);
         A.AddEntry(k,col*nrows+row-1,-div);
         A.AddEntry(k,row,-div);
         A.AddEntry(k,col*nrows+row,4*div);
         b.push_back(div*-Tc*(exp(-10*(col*height-length/2)*(col*height-length/2))-2)); }
         
       else if (row==nrows-1 && col == 0) {
         A.AddEntry(k,(ncols-1)*nrows+row,-div);
         A.AddEntry(k,row-1,-div);
         A.AddEntry(k,nrows+row,-div);
         A.AddEntry(k,row,4*div);
         b.push_back(div*-Tc*(exp(-10*(col*height-length/2)*(col*height-length/2))-2)); 
         }
       k++;
     }
  }
  return 0;
  }

  int HeatEquation2D::Solve(std::string soln_prefix) {
    //Inital guess of zeros
    for (unsigned int n=0; n<nrows*ncols;n++)
      x.push_back(0);
    x.resize(nrows*ncols);
        
    A.ConvertToCSR();
    std::vector<int> row = A.getRow();
    std::vector<int> col = A.getCol();
    std::vector<double> val = A.getVal();
    std::vector<std::vector<double>> soln = CGSolver(val,row,col,b,x,.00001);
    
    //Writing solution to a file every ten iterations (or first and last iteration)
    std::ofstream g;
    for (unsigned int n=0; n<soln.size(); n++) {
      int curr_num = n*10;
      std::string curr_soln = std::to_string(curr_num);
      std::string soln_prefix2 = soln_prefix+curr_soln;
      g.open(soln_prefix2.c_str());
      
      std::vector<double> temp = soln[n];
      for (unsigned int n=0;n<temp.size();n++)
        g << std::fixed << std::setprecision(4) << std::scientific << temp[n] << std::endl;
      g.close();
    }
    return 0;
  }
  
