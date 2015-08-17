#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include "types.h"
#include "bitmap.h"
#include "mesh.h"
#include "vec.h"
#include "matrix.h"
#include "utility.h"
#include "graphics.h"

using namespace std;

class Shader : public IShader
{
  shared_ptr<mesh> m_mesh;
  shared_ptr<bitmap> m_diffuse;

  // fragment shader parameters
  vec3<vec2f> m_uvs;

public:
  void setMesh(shared_ptr<mesh> m)
  {
    m_mesh = m;
  }

  void setDiffuse(shared_ptr<bitmap> d)
  {
    m_diffuse = d;
  }

  virtual vec3f vertex(uint32 face, uint32 vert) override
  {
    auto f = m_mesh->face(face);
    m_uvs[vert] = m_mesh->uv(f[vert].uv);
    return m_mesh->vertice(f[vert].v);
  }

  virtual color fragment(uint32 x, uint32 y, vec3f bc) override
  {
    vec2f uv = lerp(bc, m_uvs);
    color fc = m_diffuse->getPixel(m_diffuse->width() * uv.x(), m_diffuse->height() * uv.y());

    return fc;
  }
};

void triangle(bitmap& image, 
              bitmap& texture,
              vector<vector<float>>& zbuffer,
              const vec3<vec3f>& vertices,
              const vec3<vec2f>& uvs,
              const vec3<vec3f>& normales,
              uint8 r, uint8 g, uint8 b);


int main(int argc, char** argv)
{
  assert(argc == 3);

  auto meshToRender = make_shared<mesh>();
  fstream file;
  file.open(argv[1], ios_base::in);
  meshToRender->deserialize(file);
  file.close();

  file.open(argv[2], ios_base::in | ios_base::binary);
  auto diffuse = make_shared<bitmap>(bitmap::deserialize(file));
  file.close();

  int width = 700;
  int height = 700;
  int depth = 255;
  bitmap image(width, height);
  vector<float> zbuffer;
  zbuffer.insert(zbuffer.begin(), width * height, -numeric_limits<float>::infinity());

  Shader shader;
  shader.setMesh(meshToRender);
  shader.setDiffuse(diffuse);

  for (uint32 face = 0; face < meshToRender->numFaces(); ++face) 
  {
    renderFace(face, shader, image, zbuffer);
  }

  image.serialize(cout);
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

      float z = lerp(bc, vertices[0].z(), vertices[1].z(),vertices[2].z());

      if (bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0 && z > zbuffer[y][x])
      {
        vec3f normale = lerp(bc, normales);
        float lum = dot(normale, vec3f { 0, 0, 1.0f });
        if (lum < 0)
        {
          continue;
        }

        zbuffer[y][x] = z;

        vec2f uv = lerp(bc, uvs);
        color col = texture.getPixel(texture.width() * uv.x(), texture.height() * uv.y());

        image.setPixel(x, y, col.r * lum, col.g * lum, col.b * lum);
      }
    }
  }
}
