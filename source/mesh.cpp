#include <cassert>
#include <istream>
#include <string>
#include "types.h"
#include "vector3.h"
#include "mesh.h"

using namespace std;

void Mesh::Deserialize(istream& stream)
{
  string str;
  stream >> str >> str;
  assert(str == "num_vertices");
  uint32 num_vertices;
  stream >> num_vertices;

  stream >> str >> str;
  assert(str == "num_faces");
  uint32 num_faces;
  stream >> num_faces;

  vertices.clear();
  vertices.reserve(num_vertices);
  faces.clear();
  faces.reserve(num_faces);

  while (stream >> str)
  {
    if (str == "v")
    {
      float x, y, z;
      stream >> x >> y >> z;
      vertices.push_back(Vector3(x, y, z));
    }
    else if (str == "f")
    {
      uint32 v0, v1, v2;
      stream >> v0;
      stream.ignore(20, ' ');
      stream >> v1;
      stream.ignore(20, ' ');
      stream >> v2;
      faces.push_back(array<uint32, 3> {{v0, v1, v2}});
    }
  }

  assert(vertices.size() == num_vertices);
  assert(faces.size() == num_faces);
}
