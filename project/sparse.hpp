#ifndef SPARSE_HPP
#define SPARSE_HPP

#include <vector>

class SparseMatrix
{
  private:
    std::vector<int> i_idx;
    std::vector<int> j_idx;
    std::vector<double> a;
    int ncols;
    int nrows;

  public:
    /* Method to add entry to matrix in COO format */
    void AddEntry(int i, int j, double val);

    /* Method to convert COO matrix to CSR format using provided function */
    void ConvertToCSR();

    void getVec(void);

    /* Additional public methods to return the current state of 
    row, column, and value pointers */
    std::vector<int> getRow();
    std::vector<int> getCol();
    std::vector<double> getVal();
};

#endif /* SPARSE_HPP */
