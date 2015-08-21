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

void renderFace(uint32 face,
                IShader& shader,
                bitmap& image,
                std::vector<float>& zbuffer)
{
  vec3<vec3f> clipCoords;
  for (uint32 i = 0; i < 3; ++i)
  {
    clipCoords[i] = shader.vertex(face, i);
  }

  // define clip to screen space transformation matrix
  mat4x4f screenSpace = mat4x4f::identity();
  float halfWidth     = image.width()  * 0.5f;
  float halfHeight    = image.height() * 0.5f;
  screenSpace[0][0]   = halfWidth;
  screenSpace[0][3]   = halfWidth  + 0.5f;
  screenSpace[1][1]   = halfHeight;
  screenSpace[1][3]   = halfHeight + 0.5f;

  // convert clip space coords to screen space and find AABB of the triangle
  vec3<vec3f> screenCoords; 
  vec2i minAABB(numeric_limits<int32>::max());
  vec2i maxAABB(-numeric_limits<int32>::min());

  for (uint32 i = 0; i < 3; ++i)
  {
    screenCoords[i] = project<3>(screenSpace * embed<4>(clipCoords[i], 1.0f));

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
      // TODO: handle this case as opengl, by splitting half-visible triangles
      if (x < 0 || x >= image.width() || y < 0 || y >= image.height())
        continue;

      vec3f bc = barycentricCoords(vec2f{ float(x), float(y) }, screenCoords);
      if (bc.x() < 0 || bc.y() < 0 || bc.z() < 0)
        continue;

      float z = lerp(bc, screenCoords[0].z(),
                     screenCoords[1].z(), screenCoords[2].z());
      uint32 zbi = y * image.width() + x;
      if (z > zbuffer[zbi])
      {
        zbuffer[zbi] = z; 
        color c = shader.fragment(x, y, bc);
        image.setPixel(x, y, c.r, c.g, c.b);
      }
    }
  }
}

mat4x4f lookAt(vec3f eye, vec3f up, vec3f target)
{
  vec3f zaxis = (eye - target).unit();
  vec3f xaxis = cross(up, zaxis).unit();
  vec3f yaxis = cross(zaxis, xaxis);

  mat4x4f orientation
  {
    { xaxis.x(), xaxis.y(), xaxis.z(), 0 },
    { yaxis.x(), yaxis.y(), yaxis.z(), 0 },
    { zaxis.x(), zaxis.y(), zaxis.z(), 0 },
    {         0,         0,         0, 1 }
  };

  mat4x4f translation
  {
    { 1, 0, 0, -eye.x() },
    { 0, 1, 0, -eye.y() },
    { 0, 0, 1, -eye.z() },
    { 0, 0, 0, 1        }
  };

  return orientation * translation;
}
