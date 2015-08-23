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

  mat4x4f m_world;
  mat4x4f m_view;
  mat4x4f m_projection;

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

  void setWorld(mat4x4f world)
  {
    m_world = world;
  }

  void setView(mat4x4f view)
  {
    m_view = view;
  }

  void setProjection(mat4x4f projection)
  {
    m_projection = projection;
  }

  virtual vec4f vertex(uint32 face, uint32 vert) override
  {
    auto f = m_mesh->face(face);
    m_uvs[vert] = m_mesh->uv(f[vert].uv);
    vec3f v = m_mesh->vertice(f[vert].v);
    return m_projection * m_view * m_world * embed<4>(v, 1.0f);
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

  int width = 500;
  int height = 500;
  bitmap image(width, height);
  vector<float> zbuffer;
  zbuffer.insert(zbuffer.begin(), width * height, numeric_limits<float>::infinity());

  Shader shader;
  shader.setMesh(meshToRender);
  shader.setDiffuse(diffuse);

  mat4x4f world = mat4x4f::identity();
  shader.setWorld(world);

  vec3f eye    { 0, 0, 3 };
  vec3f up     { 0, 1, 0 };
  vec3f target { 0, 0, 0 };
  mat4x4f view = lookAt(eye, up, target);
  shader.setView(view);

  mat4x4f proj = projection(45, float(width) / height, 0.3f, 1000);
  //mat4x4f proj = mat4x4f::identity();
  shader.setProjection(proj);

  for (uint32 face = 0; face < meshToRender->numFaces(); ++face) 
  {
    renderFace(face, shader, image, zbuffer);
  }

  image.serialize(cout);
}

