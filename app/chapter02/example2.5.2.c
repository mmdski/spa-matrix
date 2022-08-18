#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

void
solve_system(SPAMatrix *soln_ptr, SPAMatrix a, SPAMatrix b) {

  assert(soln_ptr && a && b);

  SPAMatrixSize a_size = spa_mat_size(a);
  SPAMatrixSize b_size = spa_mat_size(b);

  assert(a_size.n_rows == b_size.n_rows);
  assert(b_size.n_cols == 1); // only a single result column

  // set soln to null
  SPAMatrix soln = *soln_ptr;
  soln           = NULL;

  SPAMatrix e_c = NULL;
  spa_mat_new_colcat(&e_c, a, b);
  SPAMatrixSize e_c_size = spa_mat_size(e_c);

  spa_gauss_jordan_elim(e_c, spa_mat_prow_exch_max);

  size_t *basic_col_nos = malloc(e_c_size.n_cols * sizeof(size_t));
  size_t  n_basic_cols  = spa_gauss_basic_col_nos(e_c, basic_col_nos);

  // the c column vector in [E|c] is basic. the system is inconsistent and has
  // no solution
  if (basic_col_nos[n_basic_cols - 1] == e_c_size.n_cols) {
    goto free_mem;
  }

  size_t  n_free_cols  = e_c_size.n_cols - n_basic_cols;
  size_t *free_col_nos = malloc(sizeof(size_t) * n_free_cols);
  spa_gauss_free_col_nos(free_col_nos, n_free_cols, basic_col_nos);
  spa_mat_new(&soln, a_size.n_cols, n_free_cols);
  spa_gauss_part_solns(soln, e_c, free_col_nos);
  *soln_ptr = soln;

  free(free_col_nos);
free_mem:
  free(basic_col_nos);
  spa_mat_free(&e_c);
}

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]) {
    2, 4, 6,
    1, 2, 3,
    1, 0, 1,
    2, 4, 0
  }, 4, 3);
  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]) {
     2,
     1,
    -3,
     8
  }, 4, 1);
  // clang-format on

  SPAMatrix nonhomogeneous_soln = NULL;
  solve_system(&nonhomogeneous_soln, a, b);
  puts("The solution to [A|b] is");
  spa_mat_print(nonhomogeneous_soln);
  spa_mat_free(&nonhomogeneous_soln);

  spa_mat_free(&b);
  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
