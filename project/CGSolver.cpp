#include <iostream>
#include <vector>

#include "matvecops.hpp"
#include "CGSolver.hpp"

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol) {

  double constant = -1;
  std::vector<double> test = MatMult(val, row_ptr, col_idx, x);
  //std::vector<double> test2(row_ptr.size()-1);
 // for (unsigned int n=0; n<test2.size(); n++)
   // test2[n] = 1;
  for (unsigned int n=0; n<test.size(); n++)
        std::cout << "test " << test[n]<< std::endl;
  std::vector<double> test2 = scalMult(constant, test);
  for (unsigned int n=0; n<test2.size(); n++)
      std::cout << "-test2 " << test2[n]<< std::endl;
  for (unsigned int n=0; n<b.size(); n++)
      std::cout << "b " << b[n]<< std::endl;
  std::vector<double> r0 = vecAdd(b, test2);
  
  //std::vector<double> a(row_ptr.size()-1);
  for (unsigned int n=0; n<r0.size(); n++)
    std::cout << "r0 " << r0[n]<< std::endl;
  //std::cout << "r0 is " << r0 << std::endl;
  //std::vector<double> r0 = vecAdd(b, scalMult(constant, MatMult(val, row_ptr, col_idx, x)));
  double L2normr0 = L2norm(r0);
  std::vector<double> p_old = r0;
  /*for (unsigned int n=0; n<p0.size(); n++)
          std::cout << "p0 " << p0[n]<< std::endl;*/
  unsigned int niter = 0;
  std::vector<double> r_old = r0;
  
  while (niter < row_ptr.size()) {
    niter = niter+1;
    std::vector<double> mat = MatMult(val, row_ptr, col_idx, p_old);
    double denom = dot(p_old, mat);
    double alpha = (dot(r_old, r_old))/denom;
    std::cout << "alpha " << alpha << std::endl; 
    std::vector<double> holder = scalMult(alpha, p_old);
    //u_n+1
    x = vecAdd(x, holder);
    for (unsigned int n=0; n<x.size(); n++)
        std::cout << "x " << x[n]<< std::endl;
    //r_n+1
    std::vector<double> scal = scalMult(-alpha, mat);
    double betadenom = L2norm(r_old)*L2norm(r_old);
    std::vector<double> r_new = vecAdd(r_old, scal);
    double L2normr = L2norm(r_new);
    std::cout << "quotient: " << L2normr/L2normr0 << std::endl;
    if (L2normr/L2normr0 < tol) {
      break;
    }
    double beta = (L2normr*L2normr)/betadenom;
    scal = scalMult(beta,p_old);
    std::vector<double> p_new = vecAdd(r_new, scal);
    r_old = r_new;
    p_old = p_new;
  }
  for(unsigned int n=0; n<x.size(); n++)
    std::cout << "x[" << n << "] = " << x[n] << std::endl;
  return niter;
    
}
/*
int main() {
  std::vector<double> val(4);
  val[0] = 4;val[1] = 1; val[2] = 1; val[3] = 3;
  std::vector<int> col_idx(4);
  col_idx[0] = 0; col_idx[1] = 1; col_idx[2]=0; col_idx[3]=1;
  std::vector<int> row_ptr(3);
  row_ptr[0] = 0; row_ptr[1] = 2; row_ptr[2]=4;
  std::vector<double> b(2);
  b[0] = 0; b[1] = 0; 
  std::vector<double> x(2);
  x[0] = 1;x[1] = 1;
  double tol = .00001;

unsigned int niter = CGSolver(val, row_ptr, col_idx, b, x, tol);
std::cout << "SUCCESS: CG solver converged in " << niter << " iterations" << std::endl;
return 0;
}
*/
