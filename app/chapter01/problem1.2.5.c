#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){4, -8, 5, 4, -7, 4, 3, -4, 2}, 3, 3);

  SPAMatrix b = NULL;
  spa_mat_new_eye(&b, 3);

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
