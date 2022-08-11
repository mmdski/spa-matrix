#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  SPAMatrix a = NULL;
  // clang-format off
  spa_mat_new_arr(&a, (double[]){
    1, 2, 1, 3, 3,
    2, 4, 0, 4, 4,
    1, 2, 3, 5, 5,
    2, 4, 0, 4, 7
    }, 4, 5);
  // clang-format on

  puts("\nA=");
  spa_mat_print(a);

  spa_gauss_elim(a, spa_mat_prow_exch_zero);

  puts("\nA (reduced):");
  spa_mat_print(a);

  spa_mat_free(&a);

  return EXIT_SUCCESS;
}
