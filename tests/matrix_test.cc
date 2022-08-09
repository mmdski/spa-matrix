extern "C" {
#include <spam.h>
}

#include <gtest/gtest.h>

namespace {
TEST(SPAMatrix, NewFreeTest) {

  SPAMatrix m = NULL;
  ASSERT_EQ(spam_matrix_new(&m, 3, 3), SPAM_NO_ERROR);
  ASSERT_TRUE(m);

  spam_matrix_free(&m);
  ASSERT_FALSE(m);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
