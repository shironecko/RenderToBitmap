#pragma once

#include <ostream>
#include <istream>
#include "types.h"

// Let's make writing this stuff to a file easier for us,
// shall we :)
#pragma pack(push, 2)
struct bitmapFileHeader
{
  uint16 type;
  uint32 fileSize;
  uint16 reserved1;
  uint16 reserved2;
  uint32 offBits;
};
#pragma pack(pop)

struct bitmapHeader
{
  uint32 headerSize;
  int32 width;
  int32 height;
  uint16 planes;
  uint16 bitsPerPixel;
  uint32 compression;
  uint32 imageSize;
  uint32 xPixelsPerMeter;
  uint32 yPixelsPerMeter;
  uint32 clrUsed;
  uint32 clrImportant;
};

struct color
{
  uint8 b;
  uint8 g;
  uint8 r;
};

class bitmap
{
private:
  bitmapHeader m_header{};
  uint8* m_pixels;
  uint32 m_imageBytesNum;
  uint32 m_pitch;
  uint32 m_stride;

  uint32 coordsToOffset(uint32 x, uint32 y);
  color* getPixelAtCoords(uint32 x, uint32 y);

  bitmap();

public:
  static const uint16 FILE_TYPE = ('M' << 8) | 'B';

  bitmap(const bitmap& that) = delete;
  bitmap(bitmap&& that);
  bitmap(int32 width, int32 height);
  ~bitmap();

  void setPixel(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);
  color getPixel(uint32 x, uint32 y);

  int32 width();
  int32 height();

  void serialize(std::ostream& stream);
  static bitmap deserialize(std::istream& stream);
};
