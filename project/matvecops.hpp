#ifndef MATVECOPS_HPP
#define MATVECOPS_HPP

#include <vector>

/*Computes L2 norm of a given vector*/
double L2norm(std::vector<double> &v);

/*Computes dot product of vectors v1 and v2*/
double dot(std::vector<double> &v1, std::vector<double> &v2);

/*Computes scalar multiple of a vector*/
std::vector<double> scalMult(const double &val, std::vector<double> &v);

/*Adds vectors v1 and v2*/
std::vector<double> vecAdd(const std::vector<double> &v1,const std::vector<double> &v2);

/*Computes A*x, where A is given in CSR format*/
std::vector<double> MatMult(std::vector<double> &val,
                            std::vector<int> &row_ptr,
                            std::vector<int> &col_idx,
                            std::vector<double> &vec);

#endif /* MATVECOPS_HPP */

