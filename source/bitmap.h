#pragma once

#include <ostream>
#include "types.h"

// Let's make writing this stuff to a file easier for us,
// shall we :)
#pragma pack(push, 2)
struct BitmapFileHeader
{
  uint16 type;
  uint32 fileSize;
  uint16 reserved1;
  uint16 reserved2;
  uint32 offBits;
};
#pragma pack(pop)

struct BitmapHeader
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

struct Color
{
  uint8 r;
  uint8 g;
  uint8 b;
};

class Bitmap
{
private:
  BitmapHeader m_header{};
  uint8* m_pixels;
  uint32 m_imageBytesNum;
  uint32 m_pitch;
  uint32 m_stride;

  uint32 CoordsToOffset(uint32 x, uint32 y);
  Color* GetPixelAtCoords(uint32 x, uint32 y);

public:
  Bitmap(int32 width, int32 height);
  ~Bitmap();

  void SetPixel(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);
  Color GetPixel(uint32 x, uint32 y);

  void Serialize(std::ostream* stream);
};
