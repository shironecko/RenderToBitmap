#pragma once

#include <istream>
#include <vector>
#include <array>
#include "types.h"
#include "vector3.h"

class Mesh
{
public:
  std::vector<Vector3> vertices;
  std::vector<std::array<uint32, 3>> faces;

  void Deserialize(std::istream& stream);
};
