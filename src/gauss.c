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

    spa_mat_print(a);
  }
}

void
spa_gauss_jordan_elim(SPAMatrix a, SPAPivotExchFunc pivot_exch_func) {

  assert(a);

  double pivot_value, row_value, multiplier;

  for (size_t pivot_col = 1, pivot_row = 1;
       pivot_col <= a->n_cols && pivot_row <= a->n_rows;
       pivot_col++) {

    pivot_exch_func(a, pivot_row, pivot_col);
    pivot_value = spa_mat_get(a, pivot_row, pivot_col);

    // pivot value is already zero
    if (pivot_value == 0)
      continue;

    multiplier = spa_fl(1. / pivot_value);
    spa_mat_row_mult(a, pivot_row, multiplier);

    for (size_t i = pivot_row + 1; i <= a->n_rows; i++) {

      row_value = spa_mat_get(a, i, pivot_col);

      // row value in pivot column is already zero
      if (row_value == 0) {
        continue;
      }
      spa_mat_row_add_row(a, i, pivot_row, -row_value);
    }

    for (size_t i = 1; i < pivot_row; i++) {

      row_value = spa_mat_get(a, i, pivot_col);

      // row value in pivot column is already zero
      if (row_value == 0)
        continue;

      spa_mat_row_add_row(a, i, pivot_row, -row_value);
    }

    pivot_row++;
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
spa_gauss_basic_cols(SPAMatrix e, size_t *basic_cols) {

  assert(e);

  size_t i_cols = 0;

  for (size_t i = 1; i <= e->n_rows; ++i) {
    for (size_t j = 1; j <= e->n_cols; ++j) {
      if (fabs(spa_mat_get(e, i, j)) < ZERO_EPS)
        continue;
      else {
        basic_cols[i_cols++] = j;
        break;
      }
    }
  }

  if (i_cols < e->n_cols - 1)
    basic_cols[i_cols] = 0;

  return i_cols;
}
