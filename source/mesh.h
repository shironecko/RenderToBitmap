#pragma once

#include <istream>
#include <vector>
#include <array>
#include "types.h"
#include "vec.h"

struct meshFace
{
  uint32 v;
  uint32 uv;
  uint32 n;
};

class mesh
{
private:
  std::vector<vec3f> m_vertices;
  std::vector<vec3<meshFace>> m_faces;

public:
  uint32 numVertices();
  uint32 numFaces();
  
  vec3f vertice(uint32 index);
  vec3<meshFace> face(uint32 index);

  void deserialize(std::istream& stream);
};
