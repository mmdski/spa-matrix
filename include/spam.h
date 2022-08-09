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

// specified precision interface
extern double       spam_fl(double value);
extern void         spam_set_precision(unsigned int precision);
extern unsigned int spam_get_precision(void);

/* spa matrix interface */
extern int  spam_matrix_new(SPAMatrix *m_ptr,
                            size_t     m,
                            size_t     n); // initialize a new, empty matrix
extern void spam_matrix_free(SPAMatrix *m_ptr); // frees a matrix

#endif
