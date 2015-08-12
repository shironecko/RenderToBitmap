#include <ostream>
#include <cstring>
#include <cstdlib>
#include "types.h"
#include "bitmap.h"

bitmap::bitmap(int32 width, int32 height)
{
  // header setup
  m_header.headerSize = sizeof(m_header);
  m_header.width = width;
  m_header.height = height;
  m_header.planes = 1;
  m_header.bitsPerPixel = 24;

  // image metrics
  m_pitch = m_header.bitsPerPixel / 8;
  uint32 bytesWithPad = m_pitch * width + 3;
  m_stride = bytesWithPad - (bytesWithPad % 4);
  m_imageBytesNum = m_stride * abs(height);

  // image memory
  m_pixels = new uint8[m_imageBytesNum];
  memset(m_pixels, 0, m_imageBytesNum);
}

bitmap::~bitmap()
{
  delete[] m_pixels;
}

uint32 bitmap::coordsToOffset(uint32 x, uint32 y)
{
  return y * m_stride + x * m_pitch;
}

color* bitmap::getPixelAtCoords(uint32 x, uint32 y)
{
  uint32 offset = coordsToOffset(x, y);
  return reinterpret_cast<color*>(m_pixels + offset);
}

void bitmap::setPixel(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
{
  color* pixel = getPixelAtCoords(x, y);
  pixel->r = r;
  pixel->g = g;
  pixel->b = b;
}

color bitmap::getPixel(uint32 x, uint32 y)
{
  color* imagePixel = getPixelAtCoords(x, y);
  return *imagePixel;
}

int32 bitmap::width()
{
  return m_header.width;
}

int32 bitmap::height()
{
  return m_header.height;
}

void bitmap::serialize(std::ostream& stream)
{
  bitmapFileHeader fileHeader{};
  fileHeader.type = ('M' << 8) | 'B';
  fileHeader.offBits = sizeof(bitmapFileHeader) + sizeof(bitmapHeader);
  fileHeader.fileSize = fileHeader.offBits + m_imageBytesNum;

  stream.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
  stream.write(reinterpret_cast<char*>(&m_header), sizeof(m_header));
  stream.write(reinterpret_cast<char*>(m_pixels), m_imageBytesNum);
}
