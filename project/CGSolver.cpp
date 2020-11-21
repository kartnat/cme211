#include <iostream>
#include <vector>

#include "CGSolver.hpp"
#include "matvecops.hpp"

std::vector<std::vector<double>> CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol) {
/*This function is a conjugate gradient solver.  It takes a matrix
in CSR format with values, row pointer, and column vectors val,
row_ptr, and col_idx, respectively, and solves the system Ax=b.  
x is the starting guess and is modified in place until the desired
tolerance is achieved.  The function returns the solutions at specified
intervals, as a vector of vectors..*/
  
  //Solution output vector
  std::vector<std::vector<double>> soln;
  
  std::vector<double> init = MatMult(val, row_ptr, col_idx, x);  
  init = scalMult(-1, init);
  std::vector<double> r0 = vecAdd(b, init);  
  double L2normr0 = L2norm(r0);
  std::vector<double> p_old = r0;
  unsigned int niter = 0;
  std::vector<double> r_old = r0;
  
  
  // Conjugate Gradient iterations
  while (niter < row_ptr.size() - 1) {
    niter = niter+1;
    std::vector<double> mat = MatMult(val, row_ptr, col_idx, p_old);
    double denom = dot(p_old, mat);
    // alpha
    double alpha = (dot(r_old, r_old))/denom;    
    std::vector<double> holder = scalMult(alpha, p_old);
    // u_n+1
    x = vecAdd(x, holder);    
    //appending to solution vector
    if (niter==1 ||  niter % 10 == 0) {
      soln.push_back(x); }
    std::vector<double> scal = scalMult(-alpha, mat);
    double betadenom = L2norm(r_old)*L2norm(r_old);
    std::vector<double> r_new = vecAdd(r_old, scal);
    double L2normr = L2norm(r_new);
    // Condition to break
    if (L2normr/L2normr0 < tol) {
      soln.push_back(x);
      std::cout << "SUCCESS: CG solver converged in " << niter << " iterations" << std::endl;
      return soln;
    }
    // beta_n
    double beta = (L2normr*L2normr)/betadenom;
    scal = scalMult(beta,p_old);
    // p_n+1
    std::vector<double> p_new = vecAdd(r_new, scal);
    r_old = r_new;
    p_old = p_new;
  }
  return soln;
}
