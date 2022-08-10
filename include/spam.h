#ifndef SPAM_H_
#define SPAM_H_

#include <stdlib.h>

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

// spam error numbers
typedef enum { SPAM_NO_ERROR = 0, SPAM_MEM_ERROR } SPAM_ERROR_NUM;

typedef struct {
  size_t n_rows;
  size_t n_cols;
  double elements[1];
} SPAMatrix_;

typedef SPAMatrix_ *SPAMatrix;

typedef struct {
  const size_t n_rows;
  const size_t n_cols;
} SPAMatrixSize;

/* specified precision interface */
extern double       spam_fl(double value);
extern void         spam_set_precision(unsigned int precision);
extern unsigned int spam_get_precision(void);

/* spa matrix interface */

/**
 * @brief Creates a new matrix with uninitialized elements
 *
 * @param m_ptr pointer to new matrix
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int spam_mat_new(SPAMatrix *m_ptr, size_t m, size_t n);

/**
 * @brief Creates a new matrix and initializes all elements to 0
 *
 * @param m_ptr pointer to new matrix
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int spam_mat_new_zeros(SPAMatrix *m_ptr, size_t m, size_t n);

/**
 * @brief Creates a new matrix and initializes all elements to 1
 *
 * @param m_ptr pointer to new matrix
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int spam_mat_new_ones(SPAMatrix *m_ptr, size_t m, size_t n);

/**
 * @brief Creates a new matrix and initializes elements by copying from an array
 *
 * @param m_ptr pointer to new matrix
 * @param src array of doubles
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int
spam_mat_new_memcpy(SPAMatrix *m_ptr, const double *src, size_t m, size_t n);

/**
 * @brief Creates a new matrix as a copy of another matrix
 *
 * @param m_ptr pointer to new matrix
 * @param a matrix to copy
 * @return error status
 */
extern int spam_new_copy(SPAMatrix *m_ptr, SPAMatrix a);

/**
 * @brief Creates a new matrix in the shape of another
 *
 * @param m_ptr pointer to new matrix
 * @param a matrix to copy
 * @return error status
 */
extern int spam_new_like(SPAMatrix *m_ptr, SPAMatrix a);

/**
 * @brief Frees a matrix and sets m_ptr to @c NULL
 *
 * @param m_ptr pointer to matrix
 */
extern void spam_mat_free(SPAMatrix *m_ptr);

/* testing matrix properties */

/**
 * @brief Tests if two matrices are equal
 *
 * @param a a matrix
 * @param b another matrix
 * @return true if @p a and @p b are equal
 * @return false if @p a and @p b are not equal
 */
extern bool spam_mat_eq(SPAMatrix a, SPAMatrix b);

/**
 * @brief Returns the size of a matrix
 *
 * @param a a matrix
 * @return SPAMatrixSize size of @p a
 */
extern SPAMatrixSize spam_mat_size(SPAMatrix a);

#endif
