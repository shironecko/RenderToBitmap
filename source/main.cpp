#include <iostream>
#include <algorithm>
#include <cassert>
#include "bitmap.h"
#include "mesh.h"
#include "vec.h"

using namespace std;

void line(bitmap& image, int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b);
void triangle(bitmap& image, const array<vec<3>, 3>& vertices,
              uint8 r, uint8 g, uint8 b);

int main()
{
  int width = 600;
  int height = 600;
  bitmap image(width, height);

  mesh meshToRender;
  meshToRender.deserialize(cin);

  for (uint32 i = 0; i < meshToRender.numFaces(); ++i) 
  {
    auto face = meshToRender.face(i);
    float lightIntensity = 1.0f;

    array<vec<3>, 3> polygon;

    for (uint32 j = 0; j < 3; ++j) 
    {
      vec<3> v0 = meshToRender.vertice(face[j]);

      int x = (v0[0] + 1.0f) * width / 2.0f;
      int y = (v0[1] + 1.0f) * height / 2.0f;

      if (x >= width) x = width - 1;
      if (y >= height) y = height - 1;

      polygon[j][0] = x;
      polygon[j][1] = y;
    }

    array<vec<3>, 3> worldPolygon{ meshToRender.vertice(face[0]),
                                   meshToRender.vertice(face[1]),
                                   meshToRender.vertice(face[2]) };

    vec<3> lightDirection { 0, 0, -1.0f };
    vec<3> normal = cross(worldPolygon[2] - worldPolygon[0], worldPolygon[1] - worldPolygon[0]).unit();
    lightIntensity = dot(lightDirection, normal);
    if (lightIntensity <= 0)
    {
      continue;
    }

    triangle(image, polygon, 255 * lightIntensity, 255 * lightIntensity, 255 * lightIntensity);
  }

  image.serialize(cout);
}

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

void triangle(bitmap& image, const array<vec<3>, 3>& vertices,
              uint8 r, uint8 g, uint8 b)
{
  // define bounding box
  float xMin = vertices[0][0];
  float yMin = vertices[0][1];
  float xMax = vertices[0][0];
  float yMax = vertices[0][1];

  for (uint32 i = 1; i < 3; ++i)
  {
    xMin = min(xMin, vertices[i][0]);
    yMin = min(yMin, vertices[i][1]);
    xMax = max(xMax, vertices[i][0]);
    yMax = max(yMax, vertices[i][1]);
  }

  for (float x = xMin; x <= xMax; ++x)
  {
    for (float y = yMin; y <= yMax; ++y)
    {
      vec<3> bc = barycentricCoords<2>(vec<2> { x, y }, vertices);
      if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0)
      {
        continue;
      }

      image.setPixel(x, y, r, g, b);
    }
  }
}
