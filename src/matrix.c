#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

#include "matrix.h"

int
spa_mat_new(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  errno  = 0;
  *m_ptr = malloc(sizeof(**m_ptr) + sizeof(double) * (m * n - 1));
  if (!(*m_ptr))
    return SPA_MEM_ERROR;

  (*m_ptr)->n_rows = m;
  (*m_ptr)->n_cols = n;

  return SPA_NO_ERROR;
}

int
spa_mat_new_zeros(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  int new_status = spa_mat_new(m_ptr, m, n);
  if (new_status == SPA_MEM_ERROR)
    return new_status;

  for (size_t i = 0; i < m * n; ++i)
    (*m_ptr)->elements[i] = 0;

  return SPA_NO_ERROR;
}

int
spa_mat_new_ones(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  int new_status = spa_mat_new(m_ptr, m, n);
  if (new_status == SPA_MEM_ERROR)
    return new_status;

  for (size_t i = 0; i < m * n; ++i)
    (*m_ptr)->elements[i] = 1;

  return SPA_NO_ERROR;
}

int
spa_mat_new_arr(SPAMatrix *m_ptr, const double *src, size_t m, size_t n) {

  assert(m_ptr);
  assert(src);
  assert(m > 0 && n > 0);

  int new_status = spa_mat_new(m_ptr, m, n);
  if (new_status == SPA_MEM_ERROR) {
    return new_status;
  }

  for (size_t i = 0; i < m * n; ++i)
    (*m_ptr)->elements[i] = src[i];

  return SPA_NO_ERROR;
}

int
spa_mat_new_copy(SPAMatrix *m_ptr, SPAMatrix a) {

  assert(m_ptr);
  assert(a);

  int new_status = spa_mat_new_arr(m_ptr, a->elements, a->n_rows, a->n_cols);
  if (new_status == SPA_MEM_ERROR)
    return new_status;

  return SPA_NO_ERROR;
}

int
spa_mat_new_like(SPAMatrix *m_ptr, SPAMatrix a) {

  assert(m_ptr);
  assert(a);

  int new_status = spa_mat_new(m_ptr, a->n_rows, a->n_cols);
  if (new_status == SPA_MEM_ERROR)
    return new_status;

  return SPA_NO_ERROR;
}

int
spa_mat_new_eye(SPAMatrix *m_ptr, size_t n) {

  assert(m_ptr);
  assert(n > 0);

  int new_status = spa_mat_new(m_ptr, n, n);
  if (new_status == SPA_MEM_ERROR)
    return new_status;

  for (size_t i = 1; i <= n; ++i) {
    for (size_t j = 1; j <= n; ++j) {
      if (i == j)
        (*m_ptr)->elements[MAT_INDEX(n, i, j)] = 1;
      else
        (*m_ptr)->elements[MAT_INDEX(n, i, j)] = 0;
    }
  }

  return SPA_NO_ERROR;
}

// column concatenate
int
spa_mat_new_colcat(SPAMatrix *m_ptr, SPAMatrix a, SPAMatrix b) {

  assert(m_ptr);
  assert(a && b);
  assert(a->n_rows == b->n_rows);

  size_t total_columns = a->n_cols + b->n_cols;

  int new_status = spa_mat_new(m_ptr, a->n_rows, total_columns);
  if (new_status == SPA_MEM_ERROR)
    return new_status;

  spa_mat_colcat(*m_ptr, a, b);

  return SPA_NO_ERROR;
}

void
spa_mat_free(SPAMatrix *m_ptr) {

  if (!m_ptr || !(*m_ptr))
    return;

  free(*m_ptr);
  *m_ptr = NULL;
}

void
spa_mat_copy(SPAMatrix a, SPAMatrix b) {

  assert(a && b);
  assert((a->n_rows == b->n_rows) && (a->n_cols == b->n_cols));

  for (size_t i = 0; i < a->n_rows * a->n_cols; ++i)
    a->elements[i] = b->elements[i];
}

void
spa_mat_eye(SPAMatrix eye) {

  assert(eye);
  assert(eye->n_rows == eye->n_cols);

  size_t n = eye->n_cols;

  for (size_t i = 1; i <= n; ++i) {
    for (size_t j = 1; j <= n; ++j) {
      if (i == j)
        eye->elements[MAT_INDEX(n, i, j)] = 1;
      else
        eye->elements[MAT_INDEX(n, i, j)] = 0;
    }
  }
}

void
spa_mat_colcat(SPAMatrix ab, SPAMatrix a, SPAMatrix b) {

  assert(ab && a && b);

  assert(ab->n_cols == (a->n_cols + b->n_cols));
  assert((ab->n_rows == a->n_rows) && (ab->n_rows == b->n_rows));

  size_t total_columns = a->n_cols + b->n_cols;

  for (size_t i = 1; i <= a->n_rows; ++i) {

    for (size_t j = 1; j <= a->n_cols; ++j)
      ab->elements[MAT_INDEX(total_columns, i, j)] = spa_mat_get(a, i, j);

    for (size_t j = 1; j <= b->n_cols; ++j)
      ab->elements[MAT_INDEX(total_columns, i, j + a->n_cols)] =
          spa_mat_get(b, i, j);
  }
}

void
spa_mat_rowcat(SPAMatrix ab, SPAMatrix a, SPAMatrix b) {

  assert(ab && a && b);

  assert(a->n_cols == b->n_cols);
  assert(ab->n_cols == a->n_cols);
  assert(ab->n_rows == (a->n_rows + b->n_rows));

  for (size_t j = 1; j <= a->n_cols; ++j) {

    for (size_t i = 1; i <= a->n_rows; ++i)
      ab->elements[MAT_INDEX(a->n_cols, i, j)] = spa_mat_get(a, i, j);

    for (size_t i = 1; i <= b->n_rows; ++i)
      ab->elements[MAT_INDEX(b->n_cols, i + a->n_rows, j)] =
          spa_mat_get(b, i, j);
  }
}

bool
spa_mat_eq(SPAMatrix a, SPAMatrix b) {

  // return false if either a or b is NULL
  if (!a || !b)
    return false;

  if (a == b)
    return true;

  if ((a->n_rows != b->n_rows) || (a->n_cols != b->n_cols))
    return false;

  for (size_t i = 0; i < a->n_rows * a->n_cols; ++i) {
    if (a->elements[i] != b->elements[i])
      return false;
  }

  return true;
}

bool
spa_mat_isclose(SPAMatrix a, SPAMatrix b, double rtol, double atol) {

  assert(a && b);
  assert((a->n_rows == b->n_rows) || (a->n_cols == b->n_cols));
  assert(rtol > 0);
  assert(atol >= 0);

  if (a == b)
    return true;

  double a_ij, b_ij, abs_diff, max_tol;

  for (size_t i = 0; i < a->n_rows * a->n_cols; ++i) {
    a_ij     = a->elements[i];
    b_ij     = b->elements[i];
    abs_diff = fabs(a_ij - b_ij);
    max_tol  = fmax(rtol * fmax(fabs(a_ij), fabs(b_ij)), atol);
    if (abs_diff > max_tol)
      return false;
  }

  return true;
}

SPAMatrixSize
spa_mat_size(SPAMatrix a) {

  assert(a);

  SPAMatrixSize size = {.n_rows = a->n_rows, .n_cols = a->n_cols};

  return size;
}

void
spa_mat_print(SPAMatrix a) {

  assert(a);

  double element;

  for (size_t i = 1; i <= a->n_rows; ++i) {
    for (size_t j = 1; j <= a->n_cols; ++j) {
      element = a->elements[MAT_INDEX(a->n_cols, i, j)];
      if (fabs(element) < ZERO_EPS)
        element = 0;
      printf("%10g", element);
    }
    printf("\n");
  }
}

double
spa_mat_get(SPAMatrix a, size_t i, size_t j) {

  assert(a);
  assert(1 <= i && i <= a->n_rows);
  assert(1 <= j && j <= a->n_cols);

  return a->elements[MAT_INDEX(a->n_cols, i, j)];
}

void
spa_mat_set(SPAMatrix a, size_t i, size_t j, double value) {

  assert(a);
  assert(1 <= i && i <= a->n_rows);
  assert(1 <= j && j <= a->n_cols);

  a->elements[MAT_INDEX(a->n_cols, i, j)] = value;
}

double *
spa_mat_el(SPAMatrix a, size_t i, size_t j) {

  assert(a);
  assert(1 <= i && i <= a->n_rows);
  assert(1 <= j && j <= a->n_cols);

  return &a->elements[MAT_INDEX(a->n_cols, i, j)];
}

// a[i1,*] += c*a[i2,*]
void
spa_mat_row_add_row(SPAMatrix a, size_t i1, size_t i2, double c) {

  assert(a);
  assert(1 <= i1 && i1 <= a->n_rows);
  assert(1 <= i2 && i2 <= a->n_rows);

  double sum, prod;

  for (size_t j = 1; j <= a->n_cols; ++j) {
    prod = spa_fl(c * a->elements[MAT_INDEX(a->n_cols, i2, j)]);
    sum  = spa_fl(a->elements[MAT_INDEX(a->n_cols, i1, j)] + prod);
    a->elements[MAT_INDEX(a->n_cols, i1, j)] = sum;
  }
}

// a[i1,*] *= c*a[i1,*]
void
spa_mat_row_mult(SPAMatrix a, size_t i, double c) {

  assert(a);
  assert(1 <= i && i <= a->n_rows);

  for (size_t j = 1; j <= a->n_cols; ++j) {
    a->elements[MAT_INDEX(a->n_cols, i, j)] =
        spa_fl(c * a->elements[MAT_INDEX(a->n_cols, i, j)]);
  }
}

// a[i1,*] <=> c*a[i2,*]
void
spa_mat_row_exch(SPAMatrix a, size_t i1, size_t i2) {

  assert(a);
  assert(1 <= i1 && i1 <= a->n_rows);
  assert(1 <= i2 && i2 <= a->n_rows);

  if (i1 == i2)
    return;

  double tmp;

  for (size_t j = 1; j <= a->n_cols; ++j) {
    tmp = a->elements[MAT_INDEX(a->n_cols, i1, j)];
    a->elements[MAT_INDEX(a->n_cols, i1, j)] =
        a->elements[MAT_INDEX(a->n_cols, i2, j)];
    a->elements[MAT_INDEX(a->n_cols, i2, j)] = tmp;
  }
}

void
spa_mat_row_scale(SPAMatrix a) {

  assert(a);

  double abs_row_value;
  double abs_max_row_value;

  // find the number of columns to find the absolute maximum value in
  // n_cols is the minimum of the number of rows and the number of columns
  size_t n_cols = a->n_rows <= a->n_cols ? a->n_rows : a->n_cols;

  for (size_t i = 1; i <= a->n_rows; ++i) {

    abs_max_row_value = fabs(spa_mat_get(a, i, 1));

    for (size_t j = 2; j <= n_cols; ++j) {

      abs_row_value = spa_mat_get(a, i, j);

      if (abs_row_value > abs_max_row_value)
        abs_max_row_value = abs_row_value;
    }

    spa_mat_row_mult(a, i, 1. / abs_max_row_value);
  }
}

void
spa_mat_prow_exch_no(SPAMatrix a, size_t pivot_row, size_t pivot_col) {
  (void) a;
  (void) pivot_row;
  (void) pivot_col;
}

void
spa_mat_prow_exch_zero(SPAMatrix a, size_t pivot_row, size_t pivot_col) {

  assert(a);
  assert(1 <= pivot_row && pivot_row <= a->n_rows);
  assert(1 <= pivot_col && pivot_col <= a->n_cols);

  size_t i;
  for (i = pivot_row; i <= a->n_rows; ++i) {
    if (fabs(a->elements[MAT_INDEX(a->n_cols, i, pivot_col)]) > ZERO_EPS)
      break;
  }

  if (i <= a->n_rows)
    spa_mat_row_exch(a, i, pivot_row);
}

void
spa_mat_prow_exch_max(SPAMatrix a, size_t pivot_row, size_t pivot_col) {

  assert(a);
  assert(1 <= pivot_row && pivot_row <= a->n_rows);
  assert(1 <= pivot_col && pivot_col <= a->n_cols);

  size_t max_row       = pivot_row;
  double abs_max_value = fabs(spa_mat_get(a, pivot_row, pivot_col));
  double abs_value;
  for (size_t i = pivot_row + 1; i <= a->n_rows; ++i) {
    abs_value = fabs(spa_mat_get(a, i, pivot_col));
    if (abs_value > abs_max_value) {
      abs_max_value = abs_value;
      max_row       = i;
    }
  }

  spa_mat_row_exch(a, pivot_row, max_row);
}

void
spa_mat_row_extract(SPAMatrix rows, SPAMatrix a, size_t *row_nos) {

  assert(rows && a && row_nos);
  assert(rows->n_rows <= a->n_rows);
  assert(rows->n_cols == a->n_cols);

  double value;

  for (size_t i = 1; i <= rows->n_rows; ++i) {
    for (size_t j = 1; j <= rows->n_cols; ++j) {
      value = spa_mat_get(a, row_nos[i - 1], j);
      spa_mat_set(rows, i, j, value);
    }
  }
}

void
spa_mat_col_extract(SPAMatrix cols, SPAMatrix a, size_t *col_nos) {

  assert(cols && a && col_nos);
  assert(cols->n_rows == a->n_rows);
  assert(cols->n_cols <= a->n_cols);

  double value;

  for (size_t j = 1; j <= cols->n_cols; ++j) {
    for (size_t i = 1; i <= cols->n_rows; ++i) {
      value = spa_mat_get(a, i, col_nos[j - 1]);
      spa_mat_set(cols, i, j, value);
    }
  }
}

void
spa_mat_transpose(SPAMatrix b, SPAMatrix a) {

  assert(a && b);
  assert(b->n_rows == a->n_cols);
  assert(b->n_cols == a->n_rows);

  double aij;
  for (size_t i = 1; i <= a->n_rows; ++i) {
    for (size_t j = 1; j <= a->n_cols; ++j) {
      aij = spa_mat_get(a, i, j);
      spa_mat_set(b, j, i, aij);
    }
  }
}

void
spa_mat_scalar_mult(SPAMatrix b, double c, SPAMatrix a) {

  assert(b && a);
  assert(b->n_rows == a->n_rows);
  assert(b->n_cols == a->n_cols);

  double prod;
  for (size_t i = 1; i <= a->n_rows; ++i) {
    for (size_t j = 1; j <= a->n_cols; ++j) {
      prod = c * spa_mat_get(a, i, j);
      spa_mat_set(b, i, j, prod);
    }
  }
}

void
spa_mat_matrix_add(SPAMatrix c, SPAMatrix a, SPAMatrix b) {

  assert(a && b && c);
  assert(a->n_rows == b->n_rows);
  assert(a->n_cols == b->n_cols);
  assert(a->n_rows == c->n_rows);
  assert(a->n_cols == c->n_cols);

  double sum;

  for (size_t i = 1; i <= a->n_rows; ++i) {
    for (size_t j = 1; j <= b->n_cols; ++j) {
      sum = spa_mat_get(a, i, j) + spa_mat_get(b, i, j);
      spa_mat_set(c, i, j, sum);
    }
  }
}

void
spa_mat_matrix_sub(SPAMatrix c, SPAMatrix a, SPAMatrix b) {

  assert(a && b && c);
  assert(a->n_rows == b->n_rows);
  assert(a->n_cols == b->n_cols);
  assert(a->n_rows == c->n_rows);
  assert(a->n_cols == c->n_cols);

  double sum;

  for (size_t i = 1; i <= a->n_rows; ++i) {
    for (size_t j = 1; j <= b->n_cols; ++j) {
      sum = spa_mat_get(a, i, j) - spa_mat_get(b, i, j);
      spa_mat_set(c, i, j, sum);
    }
  }
}

void
spa_mat_matrix_mult(SPAMatrix b, SPAMatrix a, SPAMatrix x) {

  assert(a && b && x);
  assert(a->n_cols == x->n_rows);
  assert(b->n_rows == a->n_rows);
  assert(b->n_cols == x->n_cols);

  double sum;

  for (size_t i = 1; i <= a->n_rows; ++i) {
    for (size_t j = 1; j <= x->n_cols; ++j) {
      sum = 0;
      for (size_t k = 1; k <= a->n_cols; ++k) {
        sum += spa_mat_get(a, i, k) * spa_mat_get(x, k, j);
      }
      spa_mat_set(b, i, j, sum);
    }
  }
}
