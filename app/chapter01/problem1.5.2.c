#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){1, 1, -10, 1e5}, 2, 2);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){3, 1e5}, 2, 1);

  SPAMatrix x = NULL;
  spa_mat_new(&x, 2, 1);

  puts("Ax=b");

  puts("\nA:");
  spa_mat_print(a);

  puts("\nb:");
  spa_mat_print(b);

  // part (a) precision of 4, with partial pivoting
  puts("(a)");
  spa_precision_set(4);
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_max);
  puts("\nx: (with partial pivoting)");
  spa_mat_print(x);

  // part (c) precision of 4, with partial pivoting and row scaling
  spa_precision_set(4);
  puts("(c)");
  SPAMatrix aug = NULL;
  spa_mat_new_colcat(&aug, a, b);
  spa_mat_row_scale(aug);

  puts("\nScaled augmented matrix:");
  spa_mat_print(aug);

  spa_gauss_elim(aug, spa_mat_prow_exch_max);
  spa_gauss_back_sub(aug, x);

  puts("\nx: (with partial pivoting and row scaling)");
  spa_mat_print(x);

  // part (d) "exact", with partial pivoting
  spa_precision_set(16);
  puts("(d)");
  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_max);
  puts("\nx: (\"exact\")");
  spa_mat_print(x);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&x);
  spa_mat_free(&aug);

  return EXIT_SUCCESS;
}
