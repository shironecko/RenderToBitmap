#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <limits>
#include "bitmap.h"
#include "mesh.h"
#include "vec.h"
#include "types.h"

using namespace std;

void line(bitmap& image, int32 x0, int32 y0, int32 x1, int32 y1,
          uint8 r, uint8 g, uint8 b);
void triangle(bitmap& image, 
              bitmap& texture,
              vector<vector<float>>& zbuffer,
              const vec3<vec3f>& vertices,
              const vec3<vec2f>& uvs,
              const vec3<vec3f>& normales,
              uint8 r, uint8 g, uint8 b);

template<typename T>
T lerp3(float alpha, float beta, float omega, T a, T b, T c)
{
  return a * alpha + b * beta + c * omega;
}

int main(int argc, char** argv)
{
  assert(argc == 3);

  mesh meshToRender;
  fstream file;
  file.open(argv[1], ios_base::in);
  meshToRender.deserialize(file);
  file.close();

  file.open(argv[2], ios_base::in | ios_base::binary);
  bitmap texture = bitmap::deserialize(file);
  file.close();

  int width = 700;
  int height = 700;
  int depth = 255;
  bitmap image(width, height);
  vector<vector<float>> zbuffer;
  for (uint32 y = 0; y < height; ++y)
  {
    vector<float> row(width);
    row.insert(row.begin(), width, -numeric_limits<float>::infinity());

    zbuffer.push_back(row);
  }

  for (uint32 i = 0; i < meshToRender.numFaces(); ++i) 
  {
    auto face = meshToRender.face(i);

    vec3<vec3f> polygon;
    vec3<vec2f> uvs;
    vec3<vec3f> normales;

    for (uint32 j = 0; j < 3; ++j) 
    {
      vec3f v0 = meshToRender.vertice(face[j].v);

      int x = (v0.x() + 1.0f) * width / 2.0f;
      int y = (v0.y() + 1.0f) * height / 2.0f;
      int z = (v0.z() + 1.0f) * depth / 2.0f;

      if (x >= width) x = width - 1;
      if (y >= height) y = height - 1;

      polygon[j].x() = x;
      polygon[j].y() = y;
      polygon[j].z() = z;

      uvs[j] = meshToRender.uv(face[j].uv);
      normales[j] = meshToRender.normale(face[j].n);
    }

    array<vec3f, 3> worldPolygon { meshToRender.vertice(face[0].v),
                                   meshToRender.vertice(face[1].v),
                                   meshToRender.vertice(face[2].v) };

    vec3f lightDirection { 0, 0, -1.0f };
    vec3f normal = cross(worldPolygon[2] - worldPolygon[0], worldPolygon[1] - worldPolygon[0]).unit();
    float lit = dot(lightDirection, normal);
    if (lit <= 0)
    {
      continue;
    }

    triangle(image, texture, zbuffer, polygon, uvs, normales, 255, 255, 255);
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

void triangle(bitmap& image, 
              bitmap& texture,
              vector<vector<float>>& zbuffer,
              const vec3<vec3f>& vertices,
              const vec3<vec2f>& uvs,
              const vec3<vec3f>& normales,
              uint8 r, uint8 g, uint8 b)
{
  // define bounding box
  float xMin = vertices[0].x();
  float yMin = vertices[0].y();
  float xMax = vertices[0].x();
  float yMax = vertices[0].y();

  for (uint32 i = 1; i < 3; ++i)
  {
    xMin = min(xMin, vertices[i].x());
    yMin = min(yMin, vertices[i].y());
    xMax = max(xMax, vertices[i].x());
    yMax = max(yMax, vertices[i].y());
  }

  for (float x = xMin; x <= xMax; ++x)
  {
    for (float y = yMin; y <= yMax; ++y)
    {
      vec3f bc = barycentricCoords<2>(vec2f { x, y }, vertices);

      float z = lerp3(bc[0], bc[1], bc[2],
                      vertices[0].z(), vertices[1].z(),vertices[2].z());

      if (bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0 && z > zbuffer[y][x])
      {
        vec3f normale = lerp3(bc[0], bc[1], bc[2],
                              normales[0], normales[1], normales[2]);
        float lum = dot(normale, vec3f { 0, 0, 1.0f });
        if (lum < 0)
        {
          continue;
        }

        zbuffer[y][x] = z;

        vec2f uv = lerp3(bc[0], bc[1], bc[2],
                         uvs[0], uvs[1], uvs[2]);
        color col = texture.getPixel(texture.width() * uv.x(), texture.height() * uv.y());


        image.setPixel(x, y, col.r * lum, col.g * lum, col.b * lum);
      }
    }
  }
}
