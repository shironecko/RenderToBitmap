#include <iostream>
#include <algorithm>
#include <cassert>
#include "bitmap.h"
#include "mesh.h"

using namespace std;

void line(Bitmap& image, int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b);

float triangleArea(Vector3 va,
                   Vector3 vb,
                   Vector3 vc)
{
  float a = (vb - va).Length();
}

int main()
{
  Mesh mesh;
  mesh.Deserialize(cin);
  int width = 800;
  int height = 801;
  Bitmap image(width, height);

  for (uint32 i = 0; i < mesh.faces.size(); ++i) 
  {
    auto& face = mesh.faces[i];

    for (uint32 j = 0; j < 3; ++j) 
    {
      Vector3 v0 = mesh.vertices[face[j]];
      Vector3 v1 = mesh.vertices[face[(j + 1) % 3]];

      int x0 = (v0.x + 1.0f) * width / 2.0f;
      int y0 = (v0.y + 1.0f) * height / 2.0f;
      int x1 = (v1.x + 1.0f) * width / 2.0f;
      int y1 = (v1.y + 1.0f) * height / 2.0f;

      if (x0 >= width) x0 = width - 1;
      if (x1 >= width) x1 = width - 1;
      if (y0 >= height) y0 = height - 1;
      if (y1 >= height) y1 = height - 1;

      line(image, x0, y0, x1, y1, 0, 0, 255);
    }
  }

  image.Serialize(&cout);
}

void line(Bitmap& image,
          int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b) 
{
  assert(x0 >= 0 && x0 < image.GetWidth());
  assert(x1 >= 0 && x1 < image.GetWidth());
  assert(y0 >= 0 && y0 < image.GetHeight());
  assert(y1 >= 0 && y1 < image.GetHeight());

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
      image.SetPixel(y, x, r, g, b);
    } 
    else 
    {
      image.SetPixel(x, y, r, g, b);
    }
  }
}

