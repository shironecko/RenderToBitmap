#include "vec.h"

vec<3> cross(const vec<3>& a, const vec<3>& b)
{
  vec<3> result;
  result[0] = a[1] * b[2] - a[2] * b[1];
  result[1] = a[2] * b[0] - a[0] * b[2];
  result[2] = a[0] * b[1] - a[1] * b[0];

  return result;
}

