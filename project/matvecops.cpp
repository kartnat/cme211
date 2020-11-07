    #include <iostream>
#include <math.h>
#include <vector>

double L2norm(std::vector<double> &v) {
  double sum = 0;
  for(unsigned int n=0; n< v.size(); n++)
    sum += v[n]*v[n];
  double val = pow(sum,0.5);
  return val;
}

double dot(std::vector<double> &v1, std::vector<double> &v2) {
  double sum=0;
  for(unsigned int n=0; n<v1.size(); n++)
    sum += v1[n]*v2[n];
  return sum;
}

std::vector<double> scalMult(const double &val, std::vector<double> &v) {
  std::vector<double> u(v.size());
  for (unsigned int n=0; n<v.size(); n++)
    u[n] = val*v[n];
  return u;
}

std::vector<double> vecAdd(const std::vector<double> &v1,const std::vector<double> &v2) {
  std::vector<double> sum(v1.size());
  for (unsigned int n=0; n<v1.size(); n++)
      sum[n] = v1[n]+v2[n];
  return sum;
}

std::vector<double> MatMult(std::vector<double> &val,
                            std::vector<int> &row_ptr,
                            std::vector<int> &col_idx,
                            std::vector<double> &vec) {
  std::vector<double> v(vec.size());
  for(unsigned int n=0; n<vec.size();n++)
    v[n] = 0;
  // Computing the vector v=A*x component by component
  for(unsigned int n=0; n<row_ptr.size()-1; n++)
    for(int k=row_ptr[n]; k<row_ptr[n+1]; k++)
      v[n] += val[k]*vec[col_idx[k]];
      
  return v;
}


