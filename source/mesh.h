#pragma once

#include <istream>
#include <vector>
#include "types.h"
#include "vector3.h"

class Mesh
{
public:
  std::vector<Vector3<float>> vertices;
  std::vector<Vector3<uint32>> faces;

  void Deserialize(std::istream& stream);
};
