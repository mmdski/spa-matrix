#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  size_t n_rows     = 2;
  size_t n_cols     = 2;
  double a_values[] = {-pow(10, -4), 1, 1, 1};
  double b_values[] = {1, 2};

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, a_values, n_rows, n_cols);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, b_values, n_rows, 1);

  SPAMatrix x = NULL;

  puts("Ax=b");

  puts("\nA:");
  spa_mat_print(a);

  puts("\nb:");
  spa_mat_print(b);

  // "exact" solution
  spa_gauss_solve(a, b, &x, spa_mat_pivot_zero_exch);
  puts("\nx: (\"exact\")");
  spa_mat_print(x);

  // precision of 3, no partial pivoting
  spa_set_precision(3);
  spa_gauss_solve(a, b, &x, spa_mat_pivot_no_exch);
  puts("\nx: (no partial pivoting)");
  spa_mat_print(x);

  // precision of 3, with partial pivoting
  spa_gauss_solve(a, b, &x, spa_mat_pivot_max_exch);
  puts("\nx: (with partial pivoting)");
  spa_mat_print(x);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&x);

  return EXIT_SUCCESS;
}
