extern "C" {
#include <spam.h>
}

#include <gtest/gtest.h>

namespace {
TEST(Fl, SetPrecisionTest) {
  unsigned int precision;
  (void) precision;

  precision = 16;
  spam_set_precision(precision);
  ASSERT_EQ(spam_get_precision(), precision);

  precision = 3;
  spam_set_precision(precision);
  ASSERT_EQ(spam_get_precision(), precision);
}

TEST(Fl, ComputeTest) {
  spam_set_precision(2);
  ASSERT_FLOAT_EQ(spam_fl(3. / 80.), 0.038);

  spam_set_precision(3);
  ASSERT_FLOAT_EQ(spam_fl(89. / 47.), 1.89);
  ASSERT_FLOAT_EQ(spam_fl(1.89 * 47), 88.8);
  ASSERT_FLOAT_EQ(spam_fl(1.89 * 28), 52.9);
  ASSERT_FLOAT_EQ(spam_fl(1.89 * 19), 35.9);
  ASSERT_FLOAT_EQ(spam_fl(.1 / .1), 1.);
  ASSERT_FLOAT_EQ(spam_fl(-9. / 47.), -.191);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
