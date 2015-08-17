#pragma once

#include <vector>
#include "types.h"
#include "vec.h"
#include "matrix.h"
#include "bitmap.h"

class IShader
{
public:
  virtual vec3f vertex(uint32 face, uint32 vert) = 0;
  virtual color fragment(uint32 x, uint32 y,
                         vec3f barycentricCoords) = 0;
};

void line(bitmap& image, int32 x0, int32 y0, int32 x1, int32 y1, uint8 r, uint8 g, uint8 b);
void renderFace(uint32 face, IShader& shader, bitmap& image, std::vector<float>& zbuffer);

