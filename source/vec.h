#pragma once

#include <cmath>
#include <cassert>
#include <array>
#include "types.h"

template<uint32 N, typename T>
class vec
{
private:
  T m_data[N];

public:
  vec() { }

  vec(std::initializer_list<T> il)
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

  T& operator [] (uint32 i)
  {
    assert(i < N);
    return m_data[i];
  }

  T operator [] (uint32 i) const
  {
    assert(i < N);
    return m_data[i];
  }

  T& x()
  {
    static_assert(N == 2 || N == 3, "The x() method is defined only for vectors of size 2 or 3!");
    return (*this)[0];
  }

  T x() const
  {
    static_assert(N == 2 || N == 3, "The x() method is defined only for vectors of size 2 or 3!");
    return (*this)[0];
  }

  T& y()
  {
    static_assert(N == 2 || N == 3, "The y() method is defined only for vectors of size 2 or 3!");
    return (*this)[1];
  }

  T y() const
  {
    static_assert(N == 2 || N == 3, "The y() method is defined only for vectors of size 2 or 3!");
    return (*this)[1];
  }

  T& z()
  {
    static_assert(N == 3, "The z() method is defined only for vectors of size 3!");
    return (*this)[2];
  }

  T z() const
  {
    static_assert(N == 3, "The z() method is defined only for vectors of size 3!");
    return (*this)[2];
  }

  T sqrLength() const
  {
    return dot(*this, *this);
  }

  T length() const
  {
    return sqrt(sqrLength());
  }

  vec<N, T> unit() const
  {
    return (*this) / length();
  }

  vec<N, T>& operator +=(const vec<N, T>& that)
  {
    for (uint32 i = 0; i < N; ++i)
    {
      (*this)[i] += that[i];
    }

    return *this;
  }
  
  vec<N, T>& operator -=(const vec<N, T>& that)
  {
    return *this += -that;
  }
};

template<typename T>
vec<3, T> cross(const vec<3, T>& a, const vec<3, T>& b)
{
  return vec<3, T> 
  {
    a.y() * b.z() - a.z() * b.y(),
    a.z() * b.x() - a.x() * b.z(),
    a.x() * b.y() - a.y() * b.x()
  };
}

template<uint32 N, typename T>
bool equal(const vec<N, T>& a, const vec<N, T>& b, float epsilon)
{
  for (uint32 i = 0; i < N; ++i)
  {
    if (abs(a[i] - b[i]) > epsilon)
    {
      return false;
    }
  }

  return true;
}

template<uint32 N, typename T>
float dot(const vec<N, T>& a, const vec<N, T>& b)
{
  float result = 0;
  for (uint32 i = 0; i < N; ++i)
  {
    result += a[i] * b[i];
  }

  return result;
}

template<uint32 N, typename T>
T triangleArea(vec<N, T> a, vec<N, T> b, vec<N, T> c)
{
  T ab = (b - a).length();
  T bc = (c - b).length();
  T ca = (a - c).length();

  T s = (ab + bc + ca) * 0.5f;

  return sqrt(s * (s - ab) * (s - bc) * (s - ca));
}

template<uint32 pointN, uint32 triangleN, typename T>
vec<3, T> barycentricCoords(vec<pointN, T> p, const std::array<vec<triangleN, T>, 3>& t)
{
  static_assert(pointN >= 2, "Point dimension count is too low!");
  static_assert(triangleN >= 2, "Triangle dimension count is too low!");

  float alpha = ((t[1][1] - t[2][1])*(p[0] - t[2][0]) + (t[2][0] - t[1][0])*(p[1] - t[2][1])) /
                ((t[1][1] - t[2][1])*(t[0][0] - t[2][0]) + (t[2][0] - t[1][0])*(t[0][1] - t[2][1]));
  float beta =  ((t[2][1] - t[0][1])*(p[0] - t[2][0]) + (t[0][0] - t[2][0])*(p[1] - t[2][1])) /
                ((t[1][1] - t[2][1])*(t[0][0] - t[2][0]) + (t[2][0] - t[1][0])*(t[0][1] - t[2][1]));
  float gamma = 1.0f - alpha - beta;

  return vec<3, T> { alpha, beta, gamma };
}

template<uint32 N, typename T>
vec<N, T> operator-(vec<N, T> vector)
{
  for (uint32 i = 0; i < N; ++i)
  {
    vector[i] = -vector[i];
  }

  return vector;
}

template<uint32 N, typename T>
vec<N, T> operator+(vec<N, T> left, const vec<N, T>& right)
{
  left += right;
  return left;
}

template<uint32 N, typename T>
vec<N, T> operator-(vec<N, T> left, const vec<N, T>& right)
{
  left -= right;
  return left;
}

template<uint32 N, typename T>
vec<N, T> operator*(vec<N, T> vector, float scalar)
{
  for (uint32 i = 0; i < N; ++i)
  {
    vector[i] *= scalar;
  }

  return vector;
}

template<uint32 N, typename T>
vec<N, T> operator/(const vec<N, T>& vector, float scalar)
{
  // TODO: think about precision
  return vector * (static_cast<T>(1) / scalar);
}

template<uint32 N, typename T>
bool operator==(const vec<N, T>& left, const vec<N, T>& right)
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

typedef vec<2, int32> vec2i;
typedef vec<2, uint32> vec2u;
typedef vec<2, float> vec2f;

typedef vec<3, int32> vec3i;
typedef vec<3, uint32> vec3u;
typedef vec<3, float> vec3f;

template<typename T>
using vec2 = vec<2, T>;

template<typename T>
using vec3 = vec<3, T>;
