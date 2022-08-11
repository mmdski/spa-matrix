#include <stdio.h>
#include <stdlib.h>

#include <spam.h>

int
main(void) {

  size_t    n_rows = 3;
  size_t    n_cols = 4;
  SPAMatrix a      = NULL;
  spa_mat_new_arr(
      &a, (double[]){1, 2, 1, 1, 2, 4, 2, 2, 3, 6, 3, 4}, n_rows, n_cols);

  puts("A=");
  spa_mat_print(a);

  SPAMatrix e = NULL;
  spa_mat_new_copy(&e, a);

  spa_gauss_elim(e, spa_mat_prow_exch_zero);

  puts("E=");
  spa_mat_print(e);

  size_t *basic_cols   = malloc(n_cols * sizeof(size_t));
  size_t  n_basic_cols = spa_gauss_basic_col_nums(e, basic_cols);
  printf("A has %zu basic columns. ", n_basic_cols);
  printf("The basic column numbers of A: ");
  size_t i = 0;
  while (basic_cols[i] != 0 && i < n_basic_cols) {
    printf("%zu ", basic_cols[i]);
    i++;
  }
  puts("");

  free(basic_cols);

  spa_mat_free(&a);
  spa_mat_free(&e);

  return EXIT_SUCCESS;
}
