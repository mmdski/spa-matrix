#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){1e-3, -1, 1, 1}, 2, 2);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){1, 0}, 2, 1);

  SPAMatrix x = NULL;

  puts("Ax=b");

  puts("\nA:");
  spa_mat_print(a);

  puts("\nb:");
  spa_mat_print(b);

  // precision of 3, without partial pivoting
  spa_precision_set(3);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_no);
  puts("\nx: (without partial pivoting)");
  spa_mat_print(x);

  // precision of 3, with partial pivoting
  spa_precision_set(3);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_max);
  puts("\nx: (with partial pivoting)");
  spa_mat_print(x);

  // "exact", with partial pivoting
  spa_precision_set(16);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_max);
  puts("\nx: (\"exact\")");
  spa_mat_print(x);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&x);

  return EXIT_SUCCESS;
}
