#include <vector>
#include <iostream>

#include "COO2CSR.hpp"
#include "sparse.hpp"

  std::vector<int> i_idx;
  std::vector<int> j_idx;
  std::vector<double> a;

  void SparseMatrix::AddEntry(int i, int j, double val) {
    i_idx.push_back(i);
    j_idx.push_back(j);
    a.push_back(val);
  }
  
  void SparseMatrix::ConvertToCSR() {
    COO2CSR(a, i_idx, j_idx);
  }
  
  std::vector<int> SparseMatrix::getRow() {
    return i_idx; }
  
  std::vector<int> SparseMatrix::getCol() {
    return j_idx; }
  
  std::vector<double> SparseMatrix::getVal() {
    return a; }
       
  
