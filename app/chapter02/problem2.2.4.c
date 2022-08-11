#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    0.1, 0.2, 0.3,
    0.4, 0.5, 0.6,
    0.7, 0.8, 0.901
  }, 3, 3);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  SPAMatrix e = NULL;
  spa_mat_new_copy(&e, a);

  // "exact" arithmetic
  spa_gauss_jordan_elim(e, spa_mat_prow_exch_max);
  puts("(a) E_A using \"exact\" arithmetic is");
  spa_mat_print(e);

  // 3-digit, no partial pivoting
  spa_precision_set(3);
  spa_mat_copy(e, a);
  spa_gauss_jordan_elim(e, spa_mat_prow_exch_no);
  puts("(b) E_A using 3-digit arithmetic and no partial pivoting is");
  spa_mat_print(e);

  spa_mat_free(&a);
  spa_mat_free(&e);

  return EXIT_SUCCESS;
}
