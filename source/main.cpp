#include <iostream>
#include <algorithm>
#include <cassert>
#include "bitmap.h"
#include "mesh.h"
#include "vec.h"

using namespace std;

void line(bitmap& image, int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b);
void triangle(bitmap& image, vec<2> a, vec<2> b, vec<2> c);

template<uint32 N>
float triangleArea(vec<N> a,
                   vec<N> b,
                   vec<N> c)
{
  float ab = (b - a).length();
  float bc = (c - b).length();
  float ca = (a - c).length();

  float s = (ab + bc + ca) * 0.5f;

  return sqrt(s * (s - ab) * (s - bc) * (s - ca));
}

template<uint32 N>
vec<3> barycentricCoords(vec<N> p,
                          const array<vec<N>, 3>& triangle)
{
  float A = triangleArea(triangle[0], triangle[1], triangle[2]);
  float x = triangleArea(triangle[1], triangle[2], p) / A;
  float y = triangleArea(triangle[0], triangle[2], p) / A;
  float z = 1.0f - x - y;

  vec<3> result;
  result[0] = x;
  result[1] = y;
  result[2] = z;
  return result;
}

int main()
{
  mesh meshToRender;
  meshToRender.deserialize(cin);
  int width = 600;
  int height = 600;
  bitmap image(width, height);

  for (uint32 i = 0; i < meshToRender.numFaces(); ++i) 
  {
    auto face = meshToRender.face(i);

    for (uint32 j = 0; j < 3; ++j) 
    {
      vec<3> v0 = meshToRender.vertice(face[j]);
      vec<3> v1 = meshToRender.vertice(face[(j + 1) % 3]);

      int x0 = (v0[0] + 1.0f) * width / 2.0f;
      int y0 = (v0[1] + 1.0f) * height / 2.0f;
      int x1 = (v1[0] + 1.0f) * width / 2.0f;
      int y1 = (v1[1] + 1.0f) * height / 2.0f;

      if (x0 >= width) x0 = width - 1;
      if (x1 >= width) x1 = width - 1;
      if (y0 >= height) y0 = height - 1;
      if (y1 >= height) y1 = height - 1;

      line(image, x0, y0, x1, y1, 0, 0, 255);
    }
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

void triangle(bitmap& image, vec<2> a, vec<2> b, vec<2> c)
{
}
