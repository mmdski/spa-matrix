#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  // clang-format off
  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, (double[]){
    1,  0,  1,  4,
    0,  1,  1,  2,
    0,  0,  0,  0
  }, 3, 4);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, (double[]){
    1,
    2,
    3
  }, 3, 1);

  SPAMatrix c = NULL;
  spa_mat_new_arr(&c, (double[]){
     1,
     2,
     0
  }, 3, 1);
  // clang-format on

  puts("A=");
  spa_mat_print(a);

  SPAMatrix ab = NULL;
  spa_mat_new_colcat(&ab, a, b);
  puts("A|b");
  spa_mat_print(ab);
  spa_gauss_elim(ab, spa_mat_prow_exch_max);
  puts("A|b reduced");
  spa_mat_print(ab);
  spa_mat_free(&ab);

  SPAMatrix ac = NULL;
  spa_mat_new_colcat(&ac, a, c);
  puts("A|c");
  spa_mat_print(ac);
  spa_gauss_elim(ac, spa_mat_prow_exch_max);
  puts("A|c reduced");
  spa_mat_print(ac);
  spa_mat_free(&ac);

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&c);

  return EXIT_SUCCESS;
}
