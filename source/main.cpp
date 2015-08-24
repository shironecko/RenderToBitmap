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
  mat4x4f m_proj;
  mat4x4f m_modelToClip;

  // fragment shader parameters
  mat4x4f m_inverseWorld;
  vec3<vec3f> m_normales;
  vec3<vec2f> m_uvs;

  vec3f m_sunlightDirection;
  vec3f m_sunlightColor;

public:
  void setMesh(shared_ptr<mesh> m)
  {
    m_mesh = m;
  }

  void setDiffuse(shared_ptr<bitmap> d)
  {
    m_diffuse = d;
  }

  void setMatrices(mat4x4f world, mat4x4f view, mat4x4f proj)
  {
    m_world = world;
    m_view  = view;
    m_proj  = proj;
    m_modelToClip = m_proj * m_view * m_world ;

    // TODO: actualy calculate inverse matrix
    m_inverseWorld = mat4x4f::identity();
  }

  void setSunlight(vec3f direction, color col, float intensity)
  {
    m_sunlightDirection = direction.unit();
    m_sunlightColor     = vec3f { col.r / 255.0f, col.g / 255.0f, col.b / 255.0f };
    m_sunlightColor     = m_sunlightColor * intensity;
  }

  virtual vec4f vertex(uint32 face, uint32 vert) override
  {
    auto f = m_mesh->face(face);
    m_uvs[vert] = m_mesh->uv(f[vert].uv);
    m_normales[vert] = project(m_inverseWorld * embed<4>(m_mesh->normale(f[vert].n)));
    vec3f v = m_mesh->vertice(f[vert].v);
    return m_modelToClip * embed<4>(v);
  }

  virtual color fragment(uint32 x, uint32 y, vec3f bc) override
  {
    vec2f uv = lerp(bc, m_uvs);
    uint32 tx = m_diffuse->width()  * uv.x();
    uint32 ty = m_diffuse->height() * uv.y();

    color tc = m_diffuse->getPixel(tx, ty);
    vec3f normal = lerp(bc, m_normales).unit();
    float ilumination = max(dot(normal, m_sunlightDirection), 0.0f);
    vec3f sun = m_sunlightColor * ilumination;

    color fc;
    fc.r = uint8(tc.r * sun[0]);
    fc.g = uint8(tc.g * sun[1]);
    fc.b = uint8(tc.b * sun[2]);

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

  int width = 720;
  int height = 720;
  bitmap image(width, height);
  vector<float> zbuffer;
  zbuffer.insert(zbuffer.begin(), width * height, numeric_limits<float>::infinity());

  Shader shader;
  shader.setMesh(meshToRender);
  shader.setDiffuse(diffuse);

  mat4x4f world = mat4x4f::identity();
  vec3f eye    { 0, 0, 3 };
  vec3f up     { 0, 1, 0 };
  vec3f target { 0, 0, 0 };
  mat4x4f view = lookAt(eye, up, target);
  mat4x4f proj = projection(45, float(width) / height, 0.3f, 1000);
  shader.setMatrices(world, view, proj);

  vec3f sunDirection { 1, 0.5, 1 };
  color sunColor;
  sunColor.r = 192;
  sunColor.g = 191;
  sunColor.b = 173;
  float sunIntensity = 1.0f;
  shader.setSunlight(sunDirection, sunColor, sunIntensity);

  for (uint32 face = 0; face < meshToRender->numFaces(); ++face) 
  {
    renderFace(face, shader, image, zbuffer);
  }

  image.serialize(cout);
}

