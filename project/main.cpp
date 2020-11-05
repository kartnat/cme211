#include <fstream>
#include <iostream> 
#include <string>
#include <vector>

#include "COO2CSR.hpp"
#include "CGSolver.hpp"

int main(int, char *argv[]) {
  std::string matFile = argv[1];
  std::ifstream f;
  f.open(matFile.c_str());
  unsigned int nrows, ncols;
  if (f.is_open()) {
  
    f >> nrows >> ncols;
    std::vector<int> col_idx;
    std::vector<int> row_idx;
    std::vector<double> val;
    int row, col;
    double value;
    while (f >> row >> col >> value) {
      row_idx.push_back(row);
      col_idx.push_back(col);
      val.push_back(value);
    }
    f.close();
    COO2CSR(val, row_idx, col_idx);
    
    std::vector<double> b(50);
    //std::cout << nrows << std::endl;
    std::vector<double> x(50);
    //b[0] = 0; b[1] = 0; x[0] = 1; x[1] = 1;
    for (unsigned int n=0;n< 50;n++)
      b[0] = 0; 
      x[0] = 1;
    //for (unsigned int n=0;n< nrows;n++)
            // std::cout << x[n] << std::endl;
    double tol = .00001;
    for (unsigned int n=0;n< row_idx.size();n++)
             std::cout << "r " << row_idx[n] << std::endl;
                 for (unsigned int n=0;n< col_idx.size();n++)
                                   std::cout << "c " << col_idx[n] << std::endl;  
                                       for (unsigned int n=0;n< val.size();n++)
                                                std::cout << "v " << val[n] << std::endl;
    unsigned int niter = CGSolver(val, row_idx, col_idx, b, x, tol);
    
            
    std::cout << "SUCCESS: CG solver converged in " << niter << " iterations" << std::endl;
    /*for (unsigned int n=0;n< row_idx.size();n++)
         std::cout << row_idx[n] << std::endl;
    for (unsigned int n=0;n< col_idx.size();n++)
                  std::cout << col_idx[n] << std::endl;  
    for (unsigned int n=0;n< val.size();n++)
         std::cout << val[n] << std::endl;*/
//    for (unsigned int n=0;n< row_idx.size();n++)
  //      std::cout << row_idx[n] << std::endl; 
  }
 
  return 0;
}
