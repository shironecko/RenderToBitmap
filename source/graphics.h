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

void renderFace(uint32 face,
                IShader& shader,
                bitmap& image,
                std::vector<float>& zbuffer);

