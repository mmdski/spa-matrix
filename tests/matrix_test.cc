extern "C" {
#include <spam.h>
}

#include <gtest/gtest.h>

namespace {
TEST(SPAMatrixTest, NewFree) {

  SPAMatrix m = NULL;
  ASSERT_EQ(spam_mat_new(&m, 3, 3), SPAM_NO_ERROR);
  ASSERT_TRUE(m);

  spam_mat_free(&m);
  ASSERT_FALSE(m);
}

TEST(SPAMatrixTest, NewMemCpy) {

  double    values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  SPAMatrix m        = NULL;
  ASSERT_EQ(spam_mat_new_memcpy(&m, values, 3, 3), SPAM_NO_ERROR);
  ASSERT_TRUE(m);

  spam_mat_free(&m);
}

TEST(SPAMatrixTest, Eq) {

  double values1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18};

  SPAMatrix a = NULL;
  spam_mat_new_memcpy(&a, values1, 3, 3);

  SPAMatrix b = NULL;
  spam_mat_new_memcpy(&b, values1, 3, 3);

  SPAMatrix c = NULL;
  spam_mat_new_memcpy(&c, values2, 3, 3);

  SPAMatrix d = NULL;
  spam_mat_new_memcpy(&d, values1, 9, 1);

  ASSERT_TRUE(spam_mat_eq(a, a));
  ASSERT_TRUE(spam_mat_eq(a, b));
  ASSERT_TRUE(spam_mat_eq(b, a));

  ASSERT_FALSE(spam_mat_eq(NULL, NULL));
  ASSERT_FALSE(spam_mat_eq(a, NULL));
  ASSERT_FALSE(spam_mat_eq(NULL, a));

  ASSERT_FALSE(spam_mat_eq(a, c));
  ASSERT_FALSE(spam_mat_eq(a, d));

  spam_mat_free(&a);
  spam_mat_free(&b);
  spam_mat_free(&c);
  spam_mat_free(&d);
}

TEST(SPAMatrixTest, Size) {

  double values1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  SPAMatrix a = NULL;
  spam_mat_new_memcpy(&a, values1, 3, 3);

  SPAMatrix b = NULL;
  spam_mat_new_memcpy(&b, values1, 9, 1);

  SPAMatrixSize a_size = spam_mat_size(a);
  ASSERT_EQ(a_size.n_rows, 3);
  ASSERT_EQ(a_size.n_cols, 3);

  SPAMatrixSize b_size = spam_mat_size(b);
  ASSERT_EQ(b_size.n_rows, 9);
  ASSERT_EQ(b_size.n_cols, 1);

  spam_mat_free(&a);
  spam_mat_free(&b);
}

} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
