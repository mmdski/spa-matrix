extern "C" {
#include <spam.h>
#include <stdlib.h>
}

#include <gtest/gtest.h>

namespace {
TEST(Chapter2Examples, BasicColumnsExample) {
  // comes from example 2.2.1

  size_t    n_rows   = 3;
  size_t    n_cols   = 4;
  double    values[] = {1, 2, 1, 1, 2, 4, 2, 2, 3, 6, 3, 4};
  SPAMatrix a        = NULL;
  spa_mat_new_arr(&a, values, n_rows, n_cols);

  SPAMatrix e = NULL;
  spa_mat_new_copy(&e, a);

  spa_gauss_reduce(e, spa_mat_pivot_zero_exch);

  size_t *basic_cols   = (size_t *) malloc(n_cols * sizeof(size_t));
  size_t  n_basic_cols = spa_gauss_basic_cols(e, basic_cols);

  ASSERT_EQ(n_basic_cols, 2);

  free(basic_cols);
  spa_mat_free(&a);
  spa_mat_free(&e);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
