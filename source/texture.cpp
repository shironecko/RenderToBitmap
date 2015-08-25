#include <cassert>
#include <cstring>
#include "texture.h"
#include "types.h"
#include "vec.h"

Texture::Texture(uint32 width, uint32 height)
  : m_width(width), m_height(height)
{
  assert(width < MAX_DIMENSION && height < MAX_DIMENSION);

  m_pixels = new vec4f[Width() * Height()] {};
}

Texture::~Texture()
{
  delete[] m_pixels;
}

Texture::Texture(Texture&& that)
  : m_pixels(that.m_pixels), m_width(that.m_width), m_height(that.m_height)
{
  that.m_pixels = nullptr;
}
