#include <iostream>
#include <vector>
#include <math.h>

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

std::vector<double> vecAdd(std::vector<double> &v1,std::vector<double> &v2) {
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
  
  for(unsigned int n=0; n<row_ptr.size()-1; n++)
    for(int k=row_ptr[n]; k<row_ptr[n+1]; k++)
      v[n] += val[k]*vec[col_idx[k]];
      
  return v;
}
/*
int main() {
  std::vector<double> val(4);
  val[0] = 5;
  val[1] = 8;
  val[2] = 3;
  val[3] = 6;
  std::vector<double> col_index(4);
  col_index[0]=0;
  col_index[1]=1;
  col_index[2]=2;
  col_index[3]=1;
  std::vector<int> row_index(5);
  row_index[0]=0; row_index[1]=0;row_index[2]=2;row_index[3]=3;row_index[4]=4;
  std::vector<double> vec(4);
  vec[0] = 1; vec[1]=1;vec[2]=1;vec[3]=1;
  //std::vector<double> v=MatMult(val, row_index,col_index,vec);
  double constant = -1;
  std::vector <double> v=scalMult(constant, val);
  //std::vector<double> v=vecAdd(val, col_index);
  for(unsigned int n=0;n<v.size(); n++)
    std::cout << "v[" << n << "] = " << v[n] << std::endl;
  std::vector<double> val(3);
  double a = L2norm(v);
  std::cout << a << std::endl;
 // return 0;
}
*/
