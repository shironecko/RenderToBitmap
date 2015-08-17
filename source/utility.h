#pragma once

#include "types.h"
#include "vec.h"

template<typename T>
T lerp(float alpha, float beta, float omega, T a, T b, T c)
{
  return a * alpha + b * beta + c * omega;
}

template<typename T>
T lerp(vec3f cooficients, T a, T b, T c)
{
  return lerp(cooficients[0], cooficients[1], cooficients[2], a, b, c);
}

template<uint32 N, typename T>
T lerp(float coefficients[N], T values[N])
{
  T result{ };
  for (uint32 i = 0; i < N; ++i)
  {
    result += values[i] * coefficients[i];
  }
}

template<uint32 N, typename T>
T lerp(vec<N, float> coefficients, vec<N, T> values)
{
  T result{0};
  for (uint32 i = 0; i < N; ++i)
  {
    result += values[i] * coefficients[i];
  }

  return result;
}
