#include <vector>
#include <limits>
#include <algorithm>
#include "graphics.h"
#include "types.h"
#include "vec.h"
#include "matrix.h"
#include "utility.h"

using namespace std;

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
  vec2i minAABB(numeric_limits<int32>::infinity());
  vec2i maxAABB(-numeric_limits<int32>::infinity());

  for (uint32 i = 0; i < 3; ++i)
  {
    screenCoords[i] = project<3>(screenSpace * embed<4>(clipCoords[i], 1.0f));

    for (uint32 j = 0; j < 2; ++j)
    {
      minAABB[j] = min(minAABB[j], int32(screenCoords[i][j]));
      maxAABB[j] = max(maxAABB[j], int32(screenCoords[i][j]));
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
