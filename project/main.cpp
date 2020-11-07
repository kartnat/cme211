#include <fstream>
#include <iomanip>
#include <iostream> 
#include <string>
#include <vector>

#include "COO2CSR.hpp"
#include "CGSolver.hpp"

int main(int argc, char *argv[]) {
/*This is the main function that invokes the conjugate gradient
solver in CGSolver.cpp.  It loads a matrix in COO format, converts 
it to CSR, solves the system and writes the solution vector to a
solution file provided in the command line when running the executable */
  
  // Usage message if there are no command line arguments
  if (argc < 2) {
    std::cout << "Usage:" << std::endl;
    std::cout << " " << argv[0] << " <input matrix file name> <output solution file name>" << std::endl; 
    return 0;
    
  }
  
  // Reading in matrix in COO format
  std::string matFile = argv[1];
  std::ifstream f;
  f.open(matFile.c_str());
  unsigned int nrows, ncols;
  if (f.is_open()) {
    // Storing dimensions
    f >> nrows >> ncols;
    std::vector<int> col_idx;
    std::vector<int> row_idx;
    std::vector<double> val;
    int row, col;
    double value;
    // row_idx: row indices; col_idx: column indices
    // value: value at (row index, column index)
    while (f >> row >> col >> value) {
      row_idx.push_back(row);
      col_idx.push_back(col);
      val.push_back(value);
    }
    f.close();
    // Converting to CSR format
    COO2CSR(val, row_idx, col_idx);
    
    std::vector<double> b;
    std::vector<double> x;
    // Initializing target vector of zeroes
    for (unsigned int n=0;n< nrows;n++)
      b.push_back(0);
    // Initializing first guess of x of ones
    for (unsigned int n=0;n< nrows;n++) 
       x.push_back(1);
    x.resize(nrows);
    b.resize(nrows);
    // Initializing tolerance level and calling the solver
    double tol = .00001;
    int niter = CGSolver(val, row_idx, col_idx, b, x, tol);
    
    // Writing solution vector to solution file
    std::string solnFile = argv[2];
    std::ofstream g;
    g.open(solnFile.c_str());
    for (unsigned int n=0;n<nrows;n++)
        g << std::fixed << std::setprecision(4) << std::scientific << x[n] << std::endl;
    g.close();
    
    // Writing number of iterations to console
    std::cout << "SUCCESS: CG solver converged in " << niter << " iterations" << std::endl;
  } 
  return 0;
}
