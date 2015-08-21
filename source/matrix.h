#pragma once

#include <cmath>
#include <cassert>
#include "types.h"
#include "vec.h"

template<uint32 TWidth, uint32 THeight, typename T>
class matrix
{
  vec<TWidth, T> m_rows[THeight];

public:
  matrix() { }

  matrix(std::initializer_list<vec<TWidth, T>> il)
  {
    auto ilIterator = il.begin();
    for (uint32 i = 0; i < THeight; ++i)
    {
      if (ilIterator != il.end())
      {
        (*this)[i] = *ilIterator;
        ++ilIterator;
      }
      else
      {
        (*this)[i] = vec<TWidth, T>::zero();
      }
    }
  }

  vec<TWidth, T>& operator[] (uint32 index)
  {
    assert(index < THeight);
    return m_rows[index];
  }

  const vec<TWidth, T>& operator[] (uint32 index) const
  {
    assert(index < THeight);
    return m_rows[index];
  }

  vec<THeight, T> column(uint32 index) const
  {
    assert(index < TWidth);
    vec<THeight, T> result;
    for (uint32 i = 0; i < THeight; ++i)
    {
      result[i] = m_rows[i][index];
    }

    return result;
  }

  void setColumn(uint32 index, vec<THeight, T> v)
  {
    assert(index < TWidth);
    for (uint32 i = 0; i < THeight; ++i)
    {
      m_rows[i][index] = v[i];
    }
  }

  static matrix<THeight, TWidth, T> identity()
  {
    matrix<THeight, TWidth, T> result;
    for (uint32 y = 0; y < THeight; ++y)
    {
      for (uint32 x = 0; x < TWidth; ++x)
      {
        result[y][x] = static_cast<T>(x == y);
      }
    }

    return result;
  }
};

typedef matrix<4, 4, float> mat4x4f;

template <uint32 TDim, typename T>
vec<TDim, T> operator *(const matrix<TDim, TDim, T>& mat,
                        const vec<TDim, T>& v)
{
  vec<TDim, T> result;
  for (uint32 i = 0; i < TDim; ++i)
  {
    result[i] = dot(mat[i], v);
  }

  return result;
}

template<uint32 THeight, uint32 TWidth, uint32 TCommonDim, typename T>
matrix<THeight, TWidth, T> operator *(
    const matrix<THeight, TCommonDim, T>& left,
    const matrix<TCommonDim, TWidth, T>& right)
{
  matrix<THeight, TWidth, T> result;
  for (uint32 y = 0; y < THeight; ++y)
  {
    for (uint32 x = 0; x < TWidth; ++x)
    {
      result[y][x] = dot(left[y], right.column(x));
    }
  }

  return result;
}
