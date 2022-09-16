#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <spam.h>

#include "matrix.h"

bool
spa_gauss_elim_step(SPAMatrix a, size_t pivot_row, size_t pivot_col) {

  double pivot_value = spa_mat_get(a, pivot_row, pivot_col);
  double row_value, multiplier;

  if (fabs(pivot_value) < ZERO_EPS) {
    return false;
  }

  for (size_t i = pivot_row + 1; i <= a->n_rows; ++i) {

    row_value = spa_mat_get(a, i, pivot_col);

    // row value in pivot column is already zero
    if (row_value == 0)
      continue;

    multiplier = -spa_fl(row_value / pivot_value);
    spa_mat_row_add_row(a, i, pivot_row, multiplier);
    spa_mat_set(a, i, pivot_row, 0);
  }

  return true;
}

void
spa_gauss_elim(SPAMatrix a, SPAPivotExchFunc pivot_exch_func) {

  assert(a);
  assert(pivot_exch_func);

  for (size_t pivot_row = 1, pivot_col = 1;
       pivot_row <= a->n_rows && pivot_col <= a->n_cols;) {

    pivot_exch_func(a, pivot_row, pivot_col);

    // if reduction step succeeds, move pivot position to next row, column
    if (spa_gauss_elim_step(a, pivot_row, pivot_col)) {
      ++pivot_row;
      ++pivot_col;
    }

    // if reduction step fails, move pivot position to same row, next colum
    else {
      ++pivot_col;
    }
  }
}

void
spa_gauss_jordan_elim(SPAMatrix a, SPAPivotExchFunc pivot_exch_func) {

  assert(a);

  double pivot_value, row_value, multiplier;

  for (size_t pivot_row = 1, pivot_col = 1;
       pivot_row <= a->n_rows && pivot_col <= a->n_cols;
       ++pivot_col) {

    pivot_exch_func(a, pivot_row, pivot_col);
    pivot_value = spa_mat_get(a, pivot_row, pivot_col);

    // pivot value is already zero
    if (fabs(pivot_value) < ZERO_EPS)
      continue;

    multiplier = spa_fl(1. / pivot_value);
    spa_mat_row_mult(a, pivot_row, multiplier);

    for (size_t i = pivot_row + 1; i <= a->n_rows; i++) {

      row_value = spa_mat_get(a, i, pivot_col);

      // row value in pivot column is already zero
      if (fabs(row_value) < ZERO_EPS)
        continue;

      spa_mat_row_add_row(a, i, pivot_row, -row_value);
    }

    for (size_t i = 1; i < pivot_row; i++) {

      row_value = spa_mat_get(a, i, pivot_col);

      // row value in pivot column is already zero
      if (fabs(row_value) < ZERO_EPS)
        continue;

      spa_mat_row_add_row(a, i, pivot_row, -row_value);
    }

    ++pivot_row;
  }
}

void
spa_gauss_back_sub(SPAMatrix g_reduced, SPAMatrix x) {

  assert(g_reduced && x);

  size_t n_cols_a = g_reduced->n_rows;
  size_t n_cols_b = g_reduced->n_cols - g_reduced->n_rows;

  assert(n_cols_b > 0);
  assert(x->n_rows == n_cols_a);
  assert(x->n_cols == n_cols_b);

  double c_value, t_value, x_sum, x_val;

  for (size_t b_col = 1; b_col <= n_cols_b; ++b_col) {
    for (size_t i = g_reduced->n_rows; i > 0; --i) {

      c_value = spa_mat_get(g_reduced, i, n_cols_a + b_col);
      x_sum   = c_value;

      for (size_t j = i + 1; j <= n_cols_a; ++j) {
        t_value = spa_mat_get(g_reduced, i, j);
        x_val   = spa_mat_get(x, j, b_col);
        x_sum -= t_value * x_val;
      }

      t_value = spa_mat_get(g_reduced, i, i);
      x_val   = spa_fl(x_sum / t_value);
      spa_mat_set(x, i, b_col, x_val);
    }
  }
}

int
spa_gauss_solve(SPAMatrix        a,
                SPAMatrix        b,
                SPAMatrix       *x_ptr,
                SPAPivotExchFunc pivot_exch_func) {

  assert(a && b && x_ptr);

  int return_status = SPA_NO_ERROR;

  SPAMatrix aug        = NULL;
  int       new_status = spa_mat_new_colcat(&aug, a, b);
  if (new_status == SPA_MEM_ERROR) {
    return_status = SPA_MEM_ERROR;
    goto fail;
  }

  if (!(*x_ptr)) {
    new_status = spa_mat_new(x_ptr, a->n_cols, b->n_cols);
    if (new_status == SPA_MEM_ERROR) {
      return_status = SPA_MEM_ERROR;
      goto fail;
    }
  } else {
    assert(((*x_ptr)->n_rows == a->n_cols) && ((*x_ptr)->n_cols == b->n_cols));
  }

  spa_gauss_elim(aug, pivot_exch_func);
  spa_gauss_back_sub(aug, *x_ptr);

fail:

  spa_mat_free(&aug);

  return return_status;
}

size_t
spa_gauss_basic_col_nos(SPAMatrix e, size_t *basic_col_nums) {

  assert(e);

  size_t i_cols = 0;

  for (size_t i = 1, j = 1; i <= e->n_rows && j <= e->n_cols; ++i) {
    while (fabs(spa_mat_get(e, i, j)) < ZERO_EPS) {
      ++j;
      if (j > e->n_cols)
        goto exit;
    }
    if (basic_col_nums) {
      basic_col_nums[i_cols] = j;
    }
    i_cols++;
  }

exit:

  // if the number of columns is less than the total number of columns in the
  // matrix, set the last entry in basic_col_nums to zero
  if ((i_cols < e->n_cols - 1) && basic_col_nums)
    basic_col_nums[i_cols] = 0;

  return i_cols;
}

void
spa_gauss_free_col_nos(size_t *free_col_nos,
                       size_t  n_free_cols,
                       size_t *basic_col_nos) {

  assert(free_col_nos && basic_col_nos);

  size_t free_col_no = 1;
  size_t i_free      = 0;
  size_t i_basic     = 0;
  while (i_free < n_free_cols) {
    while (free_col_no == basic_col_nos[i_basic]) {
      free_col_no++;
      i_basic++;
    }
    free_col_nos[i_free++] = free_col_no++;
  }
}

size_t
spa_gauss_nonzero_row_nos(SPAMatrix u, size_t *nonzero_row_nums) {

  assert(u && nonzero_row_nums);

  size_t n_nonzero_rows = 0;

  size_t j;
  for (size_t i = 1; i <= u->n_rows; ++i) {
    for (j = 1; j <= u->n_cols; ++j) {
      if (fabs(spa_mat_get(u, i, j)) > ZERO_EPS)
        break;
    }
    if (j == u->n_cols) {
      nonzero_row_nums[n_nonzero_rows++] = i;
    }
  }

  return n_nonzero_rows;
}

void
spa_gauss_part_solns(SPAMatrix part_solns,
                     SPAMatrix e_c,
                     size_t   *free_col_nos) {

  assert(part_solns && e_c && free_col_nos);
  assert(part_solns->n_rows == e_c->n_cols - 1);

  SPAMatrixSize soln_size = spa_mat_size(part_solns);
  SPAMatrixSize e_c_size  = spa_mat_size(e_c);

  // the system must be consistent. the result vector c must be a free column.
  assert(e_c_size.n_cols == free_col_nos[soln_size.n_cols - 1]);

  double value; // value to fill particular solution

  // loop over the first column in the particular solutions matrix. this column
  // will contain the trivial solution if e_c is a homogeneous system
  for (size_t i_free_var = 0, e_a_row_no = 1, i_h = 1, j_h = 1;
       i_h <= soln_size.n_rows;
       ++i_h) {

    if (i_h == free_col_nos[i_free_var]) {
      value = 0;
      i_free_var++;
    } else {
      (void) e_c_size;
      value = spa_mat_get(e_c, e_a_row_no, e_c_size.n_cols);
      // value = NAN;
      e_a_row_no++;
    }

    spa_mat_set(part_solns, i_h, j_h, value);
  }

  // loop over the rest of the columns in the particular solution matrix
  for (size_t i_free_var, col_free_var, e_a_row_no, j_h = 2;
       j_h <= soln_size.n_cols;
       ++j_h) {
    col_free_var = free_col_nos[j_h - 2];
    i_free_var   = 0;
    e_a_row_no   = 1;

    for (size_t i_h = 1; i_h <= soln_size.n_rows; ++i_h) {

      // set the element to 1 if the row is equal to the free variable number of
      // the column
      if (i_h == col_free_var) {
        value = 1;
      }

      // set the element to zero if the row is greater than the free variable
      // number of the column
      else if (i_h > col_free_var) {
        value = 0;
      }

      // if the current row number is equal to another free variable value, set
      // element to zero
      else if (i_h == free_col_nos[i_free_var]) {
        i_free_var++;
        value = 0;
      }

      else {
        value = -spa_mat_get(e_c, e_a_row_no++, col_free_var);
      }

      spa_mat_set(part_solns, i_h, j_h, value);
    }
  }
}
