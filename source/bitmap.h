#pragma once

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
  uint8 red;
  uint8 green;
  uint8 blue;
};

class Bitmap
{
private:
  BitmapHeader m_header = {};
  uint8* m_pixels;
  uint8 m_rowPadding;

public:
  Bitmap(int32 width, int32 height);
  Bitmap(int32 width, int32 height, uint8 r, uint8 g, uint8 b);
  ~Bitmap();

  void SetPixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b);
  Color GetPixel(int32 x, int32 y);
};
