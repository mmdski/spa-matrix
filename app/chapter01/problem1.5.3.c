#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){-3, 1, 10, -3}, 2, 2);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){-2, 7}, 2, 1);

  SPAMatrix x = NULL;

  puts("Ax=b");

  puts("\nA:");
  spa_mat_print(a);

  puts("\nb:");
  spa_mat_print(b);

  puts("\nx: (precision of 3, without partial pivoting)");
  spa_precision_set(3);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_no);
  spa_mat_print(x);

  puts("\nx: (precision of 3, with partial pivoting)");
  spa_precision_set(3);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_max);
  spa_mat_print(x);

  puts("\nx: (\"exact\", with partial pivoting)");
  spa_precision_set(16);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_max);
  spa_mat_print(x);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&x);

  return EXIT_SUCCESS;
}
