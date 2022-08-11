#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){-1, 3, -2, -1, 4, -3, -1, 5, -4}, 3, 3);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){1, 0, 0}, 3, 1);

  SPAMatrix aug = NULL;
  spa_mat_new_colcat(&aug, a, b);

  spa_gauss_elim(aug, spa_mat_prow_exch_zero);

  puts("Ax=b");

  puts("A:");
  spa_mat_print(a);

  puts("\nb:");
  spa_mat_print(b);

  puts("\nReduced augmented matrix:");
  spa_mat_print(aug);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&aug);

  return EXIT_SUCCESS;
}
