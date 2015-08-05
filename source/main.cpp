#include <iostream>
#include "bitmap.h"

using namespace std;

void line(Bitmap& image, uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint8 r, uint8 g, uint8 b);
int main()
{
  Bitmap image(200, 100);
  line(image, 10, 10, 190, 90, 0, 0, 255);
  image.Serialize(&cout);
}

void line(Bitmap& image,
          uint32 x0, uint32 y0, uint32 x1, uint32 y1,
          uint8 r, uint8 g, uint8 b)
{
  const float step = 0.01f;
  for (float t = 0; t < 1.0f; t += step)
  {
    image.SetPixel(uint32(x0 * (1.0f - t) + x1 * t),
                   uint32(y0 * (1.0f - t) + y1 * t),
                   r, g, b);
  }
}
