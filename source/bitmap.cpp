#include "types.h"
#include "bitmap.h"

Bitmap::Bitmap(int32 width, int32 height)
{
  // header setup
  m_header.headerSize = sizeof(m_header);
  m_header.width = width;
  m_header.height = height;
  m_header.planes = 1;
  m_header.bitsPerPixel = 24;
}

Bitmap::Bitmap(int32 width, int32 height, uint8 r, uint8 g, uint8 b)
  : Bitmap::Bitmap(width, height)
{
}

Bitmap::~Bitmap()
{
}
