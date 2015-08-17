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
    color fc = m_diffuse->getPixel(m_diffuse->width()  * uv.x(),
                                   m_diffuse->height() * uv.y());

    return fc;
  }
};

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

