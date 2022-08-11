#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>

#define ZERO_EPS 1e-10

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

/**
 * @brief SPA matrix structure
 *
 */
struct SPAMatrix_ {
  size_t n_rows;      /** Number of rows in matrix */
  size_t n_cols;      /** Number of columns in matrix */
  double elements[1]; /** Elements of matrix */
};

#endif
