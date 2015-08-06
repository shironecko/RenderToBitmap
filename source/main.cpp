#include <iostream>
#include <algorithm>
#include "bitmap.h"
#include "mesh.h"

using namespace std;

void line(Bitmap& image, int32 x0, int32 y0, int32 x1, int32 y1, uint8 r, uint8 g, uint8 b);

int main()
{
  Mesh mesh;
  mesh.Deserialize(cin);

  cout << "Num vertices: " << mesh.vertices.size() << endl;
  cout << "Num faces: " << mesh.faces.size() << endl;

  Bitmap image(200, 100);
  line(image, 10, 10, 40, 90, 0, 255, 0);
  line(image, 50, 90, 20, 10, 255, 0, 0);
  line(image, 100, 10, 190, 40, 0, 0, 255);
  line(image, 180, 40, 90, 10, 0, 255, 255);
  line(image, 50, 80, 80, 10, 255, 255, 0);
  line(image, 90, 10, 60, 80, 255, 0, 255);
  //image.Serialize(&cout);
}

void line(Bitmap& image,
          int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b) 
{
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

