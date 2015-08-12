#pragma once

#include <istream>
#include <vector>
#include <array>
#include "types.h"
#include "vec.h"

class mesh
{
private:
  std::vector<vec<3>> m_vertices;
  std::vector<std::array<uint32, 3>> m_faces;

public:
  uint32 numVertices();
  uint32 numFaces();
  
  vec<3> vertice(uint32 index);
  std::array<uint32, 3> face(uint32 index);

  void deserialize(std::istream& stream);
};
