extern "C" {
#include <spam.h>
}

#include <gtest/gtest.h>

namespace {
TEST(Fl, SetPrecisionTest) {
  unsigned int precision;
  (void) precision;

  precision = 16;
  spa_precision_set(precision);
  ASSERT_EQ(spa_precision_get(), precision);

  precision = 3;
  spa_precision_set(precision);
  ASSERT_EQ(spa_precision_get(), precision);
}

TEST(Fl, ComputeTest) {
  spa_precision_set(2);
  ASSERT_FLOAT_EQ(spa_fl(3. / 80.), 0.038);

  spa_precision_set(3);
  ASSERT_FLOAT_EQ(spa_fl(89. / 47.), 1.89);
  ASSERT_FLOAT_EQ(spa_fl(1.89 * 47), 88.8);
  ASSERT_FLOAT_EQ(spa_fl(1.89 * 28), 52.9);
  ASSERT_FLOAT_EQ(spa_fl(1.89 * 19), 35.9);
  ASSERT_FLOAT_EQ(spa_fl(.1 / .1), 1.);
  ASSERT_FLOAT_EQ(spa_fl(-9. / 47.), -.191);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
