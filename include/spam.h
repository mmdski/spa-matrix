#ifndef SPAM_H_
#define SPAM_H_

#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief SPA error number
 *
 */
typedef enum {
  SPA_NO_ERROR = 0, /** Value returned when no error occurs */
  SPA_MEM_ERROR     /** System memory error */
} SPAErrNumber;

/**
 * @brief SPA matrix object
 *
 */
typedef struct SPAMatrix_ *SPAMatrix;

/**
 * @brief SPA matrix size
 *
 */
typedef struct {
  const size_t n_rows; /** Number of rows */
  const size_t n_cols; /** Number of columns */
} SPAMatrixSize;

/**
 * @brief Pivot exchange function
 *
 */
typedef void (*SPAPivotExchFunc)(SPAMatrix a,
                                 size_t    pivot_row,
                                 size_t    pivot_col);

/* specified precision interface */

/**
 * @brief Returns a value rounded to the precision specified
 *
 * @param value value to round
 * @return double
 */
extern double spa_fl(double value);

/**
 * @brief Set the specified precision of arithmetic
 *
 * @param precision
 */
extern void spa_precision_set(unsigned int precision);

/**
 * @brief Get the precision of arithmetic
 *
 * @return unsigned int
 */
extern unsigned int spa_precision_get(void);

/* spa matrix interface */

/**
 * @brief Creates a new matrix with uninitialized elements
 *
 * @param m_ptr pointer to new matrix
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int spa_mat_new(SPAMatrix *m_ptr, size_t m, size_t n);

/**
 * @brief Creates a new matrix and initializes all elements to 0
 *
 * @param m_ptr pointer to new matrix
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int spa_mat_new_zeros(SPAMatrix *m_ptr, size_t m, size_t n);

/**
 * @brief Creates a new matrix and initializes all elements to 1
 *
 * @param m_ptr pointer to new matrix
 * @param m number of rows
 * @param n number of columns
 * @return error status
 */
extern int spa_mat_new_ones(SPAMatrix *m_ptr, size_t m, size_t n);

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
spa_mat_new_arr(SPAMatrix *m_ptr, const double *src, size_t m, size_t n);

/**
 * @brief Creates a new matrix as a copy of another matrix
 *
 * @param m_ptr pointer to new matrix
 * @param a matrix to copy
 * @return error status
 */
extern int spa_mat_new_copy(SPAMatrix *m_ptr, SPAMatrix a);

/**
 * @brief Creates a new matrix in the shape of another
 *
 * @param m_ptr pointer to new matrix
 * @param a matrix to copy
 * @return error status
 */
extern int spa_mat_new_like(SPAMatrix *m_ptr, SPAMatrix a);

/**
 * @brief Creates a new n by n identity matrix
 *
 * @param m_ptr pointer to new matrix
 * @param n number of rows and columns in the identity matrix
 * @return error status
 */
extern int spa_mat_new_eye(SPAMatrix *m_ptr, size_t n);

/**
 * @brief Creates a new matrix by concatenating columns of two matrices
 *
 * @details The number of rows in @p a and @p b must be equal.
 *
 * @param m_ptr pointer to new matrix
 * @param a a matrix
 * @param b another matrix
 * @return error status
 */
extern int spa_mat_new_colcat(SPAMatrix *m_ptr, SPAMatrix a, SPAMatrix b);

/**
 * @brief Frees a matrix and sets m_ptr to @c NULL
 *
 * @param m_ptr pointer to matrix
 */
extern void spa_mat_free(SPAMatrix *m_ptr);

/**
 * @brief Copies the contents of one matrix into another matrix
 *
 * @details
 * Copies the contents of @p b into @p a. The size of @p a and @p b must be
 * equal.
 *
 * @param a a matrix
 * @param b another matrix
 */
extern void spa_mat_copy(SPAMatrix a, SPAMatrix b);

/**
 * @brief Tests if two matrices are equal
 *
 * @param a a matrix
 * @param b another matrix
 * @return true if @p a and @p b are equal
 * @return false if @p a and @p b are not equal
 */
extern bool spa_mat_eq(SPAMatrix a, SPAMatrix b);

/**
 * @brief Returns the size of a matrix
 *
 * @param a a matrix
 * @return size of @p a
 */
extern SPAMatrixSize spa_mat_size(SPAMatrix a);

/**
 * @brief Prints a matrix to stdout
 *
 * @param a matrix to print
 */
void spa_mat_print(SPAMatrix a);

/**
 * @brief Returns the value of a matrix element
 *
 * @param a a matrix
 * @param i row of element
 * @param j columns of element
 * @return value of element at row @p i, column @p j
 */
extern double spa_mat_get(SPAMatrix a, size_t i, size_t j);

/**
 * @brief Sets a value of a matrix element
 *
 * @param a a matrix
 * @param i row of element
 * @param j column of element
 * @param value value of element
 */
extern void spa_mat_set(SPAMatrix a, size_t i, size_t j, double value);

/**
 * @brief Returns a pointer to a matrix element
 *
 * @param a a matrix
 * @param i row of element
 * @param j column of element
 * @return pointer to element
 */
extern double *spa_mat_el(SPAMatrix a, size_t i, size_t j);

/**
 * @brief Adds a constant times a row to another row
 *
 * @details
 * @code
 * a[i1,*] = a[i1,*] + c * a[i2,*]
 * @endcode
 *
 * @param a a matrix
 * @param i1 result row
 * @param i2 other row
 * @param c constant factor
 */
extern void spa_mat_row_add_row(SPAMatrix a, size_t i1, size_t i2, double c);

/**
 * @brief Multiplies a row in a matrix by a constant
 *
 * @details
 * @code
 * a[i1,*] = c * a[i1,*]
 * @endcode
 *
 * @param a a matrix
 * @param i a row
 * @param c a constant
 */
extern void spa_mat_row_mult(SPAMatrix a, size_t i, double c);

/**
 * @brief Exchanges rows in a matrix
 *
 * @param a a matrix
 * @param i1 a row to exchange
 * @param i2 another row to exchange
 */
extern void spa_mat_row_exch(SPAMatrix a, size_t i1, size_t i2);

/**
 * @brief Row scale a matrix
 *
 * @details
 * Scales rows in @p a by the absolute maximum value in the first min(m, n)
 * columns, where m is the number of rows and n is the number of columns.
 *
 * @param a a matrix
 */
extern void spa_mat_row_scale(SPAMatrix a);

/**
 * @brief Dummy pivot row exchange function
 *
 * @param a a matrix
 * @param pivot_row pivot row
 * @param pivot_col pivot column
 */
extern void
spa_mat_prow_exch_no(SPAMatrix a, size_t pivot_row, size_t pivot_col);

/**
 * @brief Exchanges pivot row if the current pivot position contains a zero
 *
 * @param a a matrix
 * @param pivot_row pivot row
 * @param pivot_col pivot column
 */
extern void
spa_mat_prow_exch_zero(SPAMatrix a, size_t pivot_row, size_t pivot_col);

/**
 * @brief Exchanges pivot row with the row with the maximum absolute value
 * element in the pivot column below the pivot row
 *
 * @param a a matrix
 * @param pivot_row pivot row
 * @param pivot_col pivot column
 */
extern void
spa_mat_prow_exch_max(SPAMatrix a, size_t pivot_row, size_t pivot_col);

/**
 * @brief Performs a single step in Gaussian elimination
 *
 * @details
 * The elimination step is attempted at the pivot position defined by @p
 * pivot_row and @p pivot_col. If elimination succeeds, the function returns @c
 * true. If the pivot position contains a zero, the function returns @c false.
 *
 * @param a a matrix
 * @param pivot_row pivot row number
 * @param pivot_col pivot column number
 * @return true if elimination step succeeds
 * @return false if elimination step fails
 */
extern bool
spa_gauss_elim_step(SPAMatrix a, size_t pivot_row, size_t pivot_col);

/**
 * @brief Reduce a matrix using the Gauss elimination method
 *
 * @param a a matrix
 * @param pivot_exch_func a matrix pivot exchange function
 */
extern void spa_gauss_elim(SPAMatrix a, SPAPivotExchFunc pivot_exch_func);

/**
 * @brief Reduce a matrix using the Gauss-Jordan elimination method
 *
 * @param a a matrix
 * @param pivot_exch_func a matrix pivot exchange function
 */
extern void spa_gauss_jordan_elim(SPAMatrix        a,
                                  SPAPivotExchFunc pivot_exch_func);

/**
 * @brief Solve for x by back substitution
 *
 * @details
 * @p g_reduced must be an augmented matrix in the fashion of @c [A|B], where @c
 * A is a coefficient matrix and @c B is a matrix of known values. The number of
 * rows in @p x must be equal to the number of columns in @c A. The number of
 * columns in @p x must be equal to the number of columns in @p B.
 *
 * @param g_reduced a Gauss-reduced matrix
 * @param x an appropriately sized matrix
 */
extern void spa_gauss_back_sub(SPAMatrix g_reduced, SPAMatrix x);

/**
 * @brief Solves a system of equations using Gaussian elimination and back
 * substitution
 *
 * @details
 * If @p *x_ptr is @c NULL, @p *x_ptr is set to a newly crated matrix.
 * Otherwise, @p *x_ptr must be an appropriately sized matrix.
 *
 * @param a a coefficient matrix
 * @param b a matrix of knowns
 * @param x_ptr a pointer to a matrix
 * @param pivot_exch_func a matrix pivot exchange function
 * @return error status
 */
extern int spa_gauss_solve(SPAMatrix        a,
                           SPAMatrix        b,
                           SPAMatrix       *x_ptr,
                           SPAPivotExchFunc pivot_exch_func);

/**
 * @brief Returns the number of basic columns in a Gauss-reduced matrix and
 * fills the basic column numbers in an array
 *
 * @details
 * @p basic_cols must be previously allocated. The size of @p basic_col_nums
 * must be equal to the number of columns in @p e. If all columns in @p e are
 * basic, then all column numbers will appear in @p basic_col_nums. Otherwise, a
 * zero-value follows the final basic column number.
 *
 * @param e a Gauss-reduced matrix
 * @param basic_col_nums a size_t array
 * @return the number of basic columns in @p e
 */
extern size_t spa_gauss_basic_col_nums(SPAMatrix e, size_t *basic_col_nums);

#endif
