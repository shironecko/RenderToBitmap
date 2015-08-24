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

  vec(T fill)
  {
    for (uint32 i = 0; i < N; ++i)
    {
      (*this)[i] = fill;
    }
  }

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

  static vec<N, T> zero()
  {
    return vec<N, T>{ };
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
    static_assert(N >= 2 || N <= 4, "The x() method is defined only for vectors of size 2 to 4!");
    return (*this)[0];
  }

  T x() const
  {
    static_assert(N >= 2 || N <= 4, "The x() method is defined only for vectors of size 2 to 4!");
    return (*this)[0];
  }

  T& y()
  {
    static_assert(N >= 2 || N <= 4, "The y() method is defined only for vectors of size 2 to 4!");
    return (*this)[1];
  }

  T y() const
  {
    static_assert(N >= 2 || N <= 4, "The y() method is defined only for vectors of size 2 to 4!");
    return (*this)[1];
  }

  T& z()
  {
    static_assert(N >= 3 && N <= 4, "The z() method is defined only for vectors of size 3 and 4!");
    return (*this)[2];
  }

  T z() const
  {
    static_assert(N >= 3 && N <= 4, "The z() method is defined only for vectors of size 3 and 4!");
    return (*this)[2];
  }

  T& w()
  {
    static_assert(N == 4, "The w() method is defined only for vectors of size 4!");
    return (*this)[3];
  }

  T w() const
  {
    static_assert(N == 4, "The z() method is defined only for vectors of size 4!");
    return (*this)[3];
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

typedef vec<2, int32> vec2i;
typedef vec<2, uint32> vec2u;
typedef vec<2, float> vec2f;

typedef vec<3, int32> vec3i;
typedef vec<3, uint32> vec3u;
typedef vec<3, float> vec3f;

typedef vec<4, float> vec4f;

template<typename T>
using vec2 = vec<2, T>;

template<typename T>
using vec3 = vec<3, T>;

template<uint32 TNewDim, uint32 TOldDim, typename T>
vec<TNewDim, T> embed(const vec<TOldDim, T> v, T fill = static_cast<T>(1))
{
  static_assert(TNewDim > TOldDim,
                "Can only embed into space with more dimensions.");
  vec<TNewDim, T> result;
  for (uint32 i = 0; i < TNewDim; ++i)
  {
    result[i] = i < TOldDim ? v[i] : fill;
  }

  return result;
}

template<typename T>
vec<3, T> project(const vec<4, T>& v)
{
  return vec<3, T>
  {
    v.x() / v.w(),
    v.y() / v.w(),
    v.z() / v.w()
  };
}

template<typename T>
vec3<T> cross(const vec3<T>& a, const vec3<T>& b)
{
  return vec3<T> 
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
vec3f barycentricCoords(vec<pointN, T> p, const vec3<vec<triangleN, T>>& t)
{
  static_assert(pointN >= 2, "Point dimension count is too low!");
  static_assert(triangleN >= 2, "Triangle dimension count is too low!");

  float alpha = ((t[1][1] - t[2][1])*(p[0] - t[2][0]) + (t[2][0] - t[1][0])*(p[1] - t[2][1])) /
                ((t[1][1] - t[2][1])*(t[0][0] - t[2][0]) + (t[2][0] - t[1][0])*(t[0][1] - t[2][1]));
  float beta =  ((t[2][1] - t[0][1])*(p[0] - t[2][0]) + (t[0][0] - t[2][0])*(p[1] - t[2][1])) /
                ((t[1][1] - t[2][1])*(t[0][0] - t[2][0]) + (t[2][0] - t[1][0])*(t[0][1] - t[2][1]));
  float gamma = 1.0f - alpha - beta;

  return vec3f { alpha, beta, gamma };
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

