#include <vector>
#include <limits>
#include <algorithm>
#include "graphics.h"
#include "types.h"
#include "vec.h"
#include "matrix.h"
#include "utility.h"

using namespace std;

void line(bitmap& image,
          int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b) 
{
  assert(x0 >= 0 && x0 < image.width());
  assert(x1 >= 0 && x1 < image.width());
  assert(y0 >= 0 && y0 < image.height());
  assert(y1 >= 0 && y1 < image.height());

  bool steep = false;
  if (abs(x0 - x1) < abs(y0 - y1)) 
  {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }

  if (x0>x1) 
  {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  for (int32 x=x0; x<=x1; x++) 
  {
    float t = (x - x0) / (float)(x1 - x0);
    int32 y = y0 * (1.0f - t) + y1 * t;
    if (steep) 
    {
      image.setPixel(y, x, r, g, b);
    } 
    else 
    {
      image.setPixel(x, y, r, g, b);
    }
  }
}

template<uint32 N, typename T>
bool clockwise(vec<N, T> a, vec<N, T> b, vec<N, T> c)
{
  static_assert(N >= 2, "Need ad least vec<2, T> to compute!");
  float s = -b.x() * a.y() + c.x() * a.y() + a.x() * b.y() - 
             c.x() * b.y() - a.x() * c.y() + b.x() * c.y();
  return s < 0;
}

void renderFace(uint32 face,
                IShader& shader,
                bitmap& image,
                std::vector<float>& zbuffer)
{
  vec3<vec4f> clipCoords;
  for (uint32 i = 0; i < 3; ++i)
  {
    clipCoords[i] = shader.vertex(face, i);
  }

  // back-face culling
  // NOTE: should I do this in clip or screen space?
  if (clockwise(clipCoords[0], clipCoords[1], clipCoords[2]))
  {
    return;
  }

  // define clip to screen space transformation matrix
  float hw = image.width()  * 0.5f;
  float hh = image.height() * 0.5f;
  mat4x4f screenSpace
  {
    { hw,   0,   0, hw + 0.5f },
    {  0,  hh,   0, hh + 0.5f },
    {  0,   0,   1,         0 },
    {  0,   0,   0,         1 }
  };

  // convert clip space coords to screen space and find AABB of the triangle
  vec3<vec3f> screenCoords; 
  vec2i minAABB(numeric_limits<int32>::max());
  vec2i maxAABB(-numeric_limits<int32>::min());

  for (uint32 i = 0; i < 3; ++i)
  {
    // very basic clipping
    for (uint32 j = 0; j < 3; ++j)
    {
      if (clipCoords[i][j] >  clipCoords[i].w() ||
          clipCoords[i][j] < -clipCoords[i].w())
      {
        return;
      }
    }

    screenCoords[i] = project(screenSpace * clipCoords[i]);

    for (uint32 j = 0; j < 2; ++j)
    {
      minAABB[j] = min(minAABB[j], int32(screenCoords[i][j]));
      // add 1.0f to maxAABB to fully cover the triangle and remove all gaps
      maxAABB[j] = max(maxAABB[j], int32(screenCoords[i][j] + 1.0f));
    }
  }

  // rasterize the triangle
  for (int32 y = minAABB.y(); y < maxAABB.y(); ++y)
  {
    for (int32 x = minAABB.x(); x < maxAABB.x(); ++x)
    {
      vec3f bc = barycentricCoords(vec2f{ float(x), float(y) }, screenCoords);
      if (bc.x() < 0 || bc.y() < 0 || bc.z() < 0)
        continue;

      float z = lerp(bc, screenCoords[0].z(),
                     screenCoords[1].z(), screenCoords[2].z());
      uint32 zbi = y * image.width() + x;
      if (z < zbuffer[zbi])
      {
        zbuffer[zbi] = z; 
        color c = shader.fragment(x, y, bc);
        image.setPixel(x, y, c.r, c.g, c.b);
      }
    }
  }
}

