#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){0.835, 0.667, 0.333, 0.266}, 2, 2);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){0.168, 0.067}, 2, 1);

  SPAMatrix x = NULL;

  puts("Ax=b\nA:");
  spa_mat_print(a);
  puts("b:");
  spa_mat_print(b);

  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_zero);
  puts("x:");
  spa_mat_print(x);

  spa_mat_set(b, 2, 1, 0.066);
  puts("b:");
  spa_mat_print(b);

  spa_gauss_solve(a, b, &x, spa_mat_prow_exch_zero);
  puts("x:");
  spa_mat_print(x);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&x);

  return EXIT_SUCCESS;
}
