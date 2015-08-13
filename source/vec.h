#pragma once

#include <array>
#include <cmath>
#include "types.h"

template<uint32 N>
class vec
{
private:
  std::array<float, N> m_array;

public:
  vec();
  vec(std::initializer_list<float> il);

  float& operator [] (uint32 i);
  float operator [] (uint32 i) const;

  float sqrLength() const;
  float length() const;
  vec<N> unit() const;

  vec<N>& operator +=(const vec<N>& that);
  vec<N>& operator -=(const vec<N>& that);
};

template<uint32 N>
bool equal(const vec<N>& a, const vec<N>& b, float epsilon);

template<uint32 N>
float dot(const vec<N>& a, const vec<N>& b);

template<uint32 N>
float triangleArea(vec<N> a, vec<N> b, vec<N> c);

template<uint32 N>
vec<3> barycentricCoords(vec<N> p, const std::array<vec<N>, 3>& triangle);

vec<3> cross(const vec<3>& a, const vec<3>& b);

template<uint32 N>
vec<N> operator-(const vec<N>& vector);

template<uint32 N>
vec<N> operator+(vec<N> left, const vec<N>& right);

template<uint32 N>
vec<N> operator-(vec<N> left, const vec<N>& right);

template<uint32 N>
vec<N> operator*(const vec<N>& vector, float scalar);

template<uint32 N>
vec<N> operator/(const vec<N>& vector, float scalar);

template<uint32 N>
bool operator==(const vec<N>& left, const vec<N>& right);

//*****Templates Implementation*****//

template<uint32 N>
bool equal(const vec<N>& a, const vec<N>& b, float epsilon)
{
  for (uint32 i = 0; i < N; ++i)
  {
    if (fabs(a[i] - b[i]) > epsilon)
    {
      return false;
    }
  }

  return true;
}

template<uint32 N>
vec<N>::vec()
{
  for (uint32 i = 0; i < N; ++i)
  {
    (*this)[i] = 0;
  }
}

template<uint32 N>
vec<N>::vec(std::initializer_list<float> il)
{
  auto ilIterator = il.begin();
  for (uint32 i = 0; i < N; ++i)
  {
    if (ilIterator != il.end())
    {
      (*this)[i] = *ilIterator;
      ++ilIterator;
    }
    else
    {
      (*this)[i] = 0;
    }
  }
}

template<uint32 N>
float& vec<N>::operator [] (uint32 i)
{
  return m_array[i];
}

template<uint32 N>
float vec<N>::operator [] (uint32 i) const
{
  return m_array[i];
}

template<uint32 N>
float vec<N>::sqrLength() const
{
  float result = 0;
  for (uint32 i = 0; i < N; ++i)
  {
    result += (*this)[i] * (*this)[i];
  }

  return result;
}

template<uint32 N>
float vec<N>::length() const
{
  return sqrt(sqrLength());
}

template<uint32 N>
vec<N> vec<N>::unit() const
{
  return *this / length();
}

template<uint32 N>
vec<N>& vec<N>::operator +=(const vec<N>& that)
{
  for (uint32 i = 0; i < N; ++i)
  {
    (*this)[i] += that[i];
  }

  return *this;
}

template<uint32 N>
vec<N>& vec<N>::operator -=(const vec<N>& that)
{
  return *this += -that;
}

template<uint32 N>
float dot(const vec<N>& a, const vec<N>& b)
{
  float result = 0;
  for (uint32 i = 0; i < N; ++i)
  {
    result += a[i] * b[i];
  }

  return result;
}

template<uint32 N>
float triangleArea(vec<N> a, vec<N> b, vec<N> c)
{
  float ab = (b - a).length();
  float bc = (c - b).length();
  float ca = (a - c).length();

  float s = (ab + bc + ca) * 0.5f;

  return sqrt(s * (s - ab) * (s - bc) * (s - ca));
}

template<uint32 pointN, uint32 triangleN>
vec<3> barycentricCoords(vec<pointN> p, const std::array<vec<triangleN>, 3>& t)
{
  static_assert(pointN >= 2, "Point dimension count is too low!");
  static_assert(triangleN >= 2, "Triangle dimension count is too low!");

  float alpha = ((t[1][1] - t[2][1])*(p[0] - t[2][0]) + (t[2][0] - t[1][0])*(p[1] - t[2][1])) /
                ((t[1][1] - t[2][1])*(t[0][0] - t[2][0]) + (t[2][0] - t[1][0])*(t[0][1] - t[2][1]));
  float beta =  ((t[2][1] - t[0][1])*(p[0] - t[2][0]) + (t[0][0] - t[2][0])*(p[1] - t[2][1])) /
                ((t[1][1] - t[2][1])*(t[0][0] - t[2][0]) + (t[2][0] - t[1][0])*(t[0][1] - t[2][1]));
  float gamma = 1.0f - alpha - beta;

  return vec<3> { alpha, beta, gamma };
}

template<uint32 N>
vec<N> operator-(const vec<N>& vector)
{
  vec<N> result;
  for (uint32 i = 0; i < N; ++i)
  {
    result[i] = -vector[i];
  }

  return result;
}

template<uint32 N>
vec<N> operator+(vec<N> left, const vec<N>& right)
{
  left += right;
  return left;
}

template<uint32 N>
vec<N> operator-(vec<N> left, const vec<N>& right)
{
  left -= right;
  return left;
}

template<uint32 N>
vec<N> operator*(const vec<N>& vector, float scalar)
{
  vec<N> result;
  for (uint32 i = 0; i < N; ++i)
  {
    result[i] = vector[i] * scalar;
  }

  return result;
}

template<uint32 N>
vec<N> operator/(const vec<N>& vector, float scalar)
{
  return vector * (1.0f / scalar);
}

template<uint32 N>
bool operator==(const vec<N>& left, const vec<N>& right)
{
  for (uint32 i = 0; i < N; ++i)
  {
    if (left[i] != right[i])
    {
      return false;
    }
  }

  return true;
}
