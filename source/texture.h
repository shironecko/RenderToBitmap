#pragma once

#include <cassert>
#include "types.h"
#include "vec.h"

class Texture
{
  vec4f* m_pixels;
  const uint32 m_width;
  const uint32 m_height;

  inline uint32 CoordsToIndex(uint32 x, uint32 y) const
  {
    assert(x < Width() && y < Height());

    return y * Width() + x;
  }

public:
  static const uint32 MAX_DIMENSION = 1 << 15;

  Texture(uint32 width, uint32 height);
  ~Texture();

  Texture(const Texture& that) = delete;
  Texture(Texture&& that);

  inline uint32 Width() const
  {
    return m_width;
  }

  inline uint32 Height() const
  {
    return m_height;
  }

  inline vec4f& Pixel(uint32 x, uint32 y)
  {
    return m_pixels[CoordsToIndex(x, y)];
  }

  inline const vec4f& Pixel(uint32 x, uint32 y) const
  {
    return m_pixels[CoordsToIndex(x, y)];
  }

  inline const vec4f& Pixel(vec2f uv) const
  {
    return Pixel(uint32(Width() * uv.x()), uint32(Height() * uv.y()));
  }
};
