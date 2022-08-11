#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "matrix.h"

unsigned int fl_precision = 16;

double
spa_fl(double value) {

  int    exp10;
  double inf;

  if (value > ZERO_EPS) {
    exp10 = (int) ceil(log10(value));
    inf   = INFINITY;
  } else if (value < -ZERO_EPS) {
    exp10 = (int) ceil(log10(-value));
    inf   = -INFINITY;
  } else {
    return 0;
  }

  double sig10    = value / pow(10, exp10);
  double to_round = nextafter(sig10 * pow(10, fl_precision), inf);
  double rounded  = round(to_round);
  double fl       = pow(10, exp10) * rounded / pow(10, fl_precision);
  return fl;
}

void
spa_precision_set(unsigned int precision) {
  assert(precision > 0);
  fl_precision = precision;
}

unsigned int
spa_precision_get(void) {
  return fl_precision;
}
