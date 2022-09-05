extern "C" {
#include <spam.h>
}

#include <gtest/gtest.h>

namespace {
TEST(SPAMatrixTest, NewFree) {

  SPAMatrix m = NULL;
  ASSERT_EQ(spa_mat_new(&m, 3, 3), SPA_NO_ERROR);
  ASSERT_TRUE(m);

  spa_mat_free(&m);
  ASSERT_FALSE(m);
}

TEST(SPAMatrixTest, NewMemCpy) {

  double    values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  SPAMatrix m        = NULL;
  ASSERT_EQ(spa_mat_new_arr(&m, values, 3, 3), SPA_NO_ERROR);
  ASSERT_TRUE(m);

  spa_mat_free(&m);
}

TEST(SPAMatrixTest, Eq) {

  double values1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18};

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, values1, 3, 3);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, values1, 3, 3);

  SPAMatrix c = NULL;
  spa_mat_new_arr(&c, values2, 3, 3);

  SPAMatrix d = NULL;
  spa_mat_new_arr(&d, values1, 9, 1);

  ASSERT_TRUE(spa_mat_eq(a, a));
  ASSERT_TRUE(spa_mat_eq(a, b));
  ASSERT_TRUE(spa_mat_eq(b, a));

  ASSERT_FALSE(spa_mat_eq(NULL, NULL));
  ASSERT_FALSE(spa_mat_eq(a, NULL));
  ASSERT_FALSE(spa_mat_eq(NULL, a));

  ASSERT_FALSE(spa_mat_eq(a, c));
  ASSERT_FALSE(spa_mat_eq(a, d));

  spa_mat_free(&a);
  spa_mat_free(&b);
  spa_mat_free(&c);
  spa_mat_free(&d);
}

TEST(SPAMatrixTest, Size) {

  double values1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, values1, 3, 3);

  SPAMatrix b = NULL;
  spa_mat_new_arr(&b, values1, 9, 1);

  SPAMatrixSize a_size = spa_mat_size(a);
  ASSERT_EQ(a_size.n_rows, 3);
  ASSERT_EQ(a_size.n_cols, 3);

  SPAMatrixSize b_size = spa_mat_size(b);
  ASSERT_EQ(b_size.n_rows, 9);
  ASSERT_EQ(b_size.n_cols, 1);

  spa_mat_free(&a);
  spa_mat_free(&b);
}

TEST(SPAMatrixTest, Transpose) {

  // clang-format off
  double values[] = {1, 2, 3, 4, 5,
                     6, 7, 8, 9, 10};
  double expected_values[] = {1, 6,
                              2, 7,
                              3, 8,
                              4, 9,
                              5, 10};
  // clang-format on

  SPAMatrix a = NULL;
  spa_mat_new_arr(&a, values, 2, 5);

  SPAMatrix b = NULL;
  spa_mat_new(&b, 5, 2);

  SPAMatrix expected = NULL;
  spa_mat_new_arr(&expected, expected_values, 5, 2);

  spa_mat_transpose(b, a);

  ASSERT_TRUE(spa_mat_eq(b, expected));

  spa_mat_free(&expected);
  spa_mat_free(&b);
  spa_mat_free(&a);
}

} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
