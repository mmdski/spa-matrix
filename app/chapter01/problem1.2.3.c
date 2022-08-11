#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){0, 4, -3, -1, 7, -5, -1, 8, -6}, 3, 3);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){3, 4, 5}, 3, 1);

  SPAMatrix x = NULL;

  puts("Ax=b");

  puts("A:");
  spa_mat_print(a);

  puts("\nb:");
  spa_mat_print(b);

  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_zero);

  puts("\nx:");
  spa_mat_print(x);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&x);

  return EXIT_SUCCESS;
}
