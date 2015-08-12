#include <cassert>
#include <istream>
#include <string>
#include <limits>
#include "types.h"
#include "vec.h"
#include "mesh.h"

using namespace std;

uint32 mesh::numVertices()
{
  return m_vertices.size();
}

uint32 mesh::numFaces()
{
  return m_faces.size();
}

vec<3> mesh::vertice(uint32 index)
{
  return m_vertices[index];
}

std::array<uint32, 3> mesh::face(uint32 index)
{
  return m_faces[index];
}

void mesh::deserialize(istream& stream)
{
  m_vertices.clear();
  m_faces.clear();
  string str;

  while (stream >> str)
  {
    if (str == "#")
    {
      stream >> str;
      if (str == "numVertices")
      {
        uint32 vertices;
        stream >> vertices;
        m_vertices.reserve(vertices);
      }
      else if (str == "numFaces")
      {
        uint32 faces;
        stream >> faces;
        m_faces.reserve(faces);
      }
      else
      {
        stream.ignore(numeric_limits<streamsize>::max(), stream.widen('\n'));
      }
    }
    else if (str == "v")
    {
      vec<3> vertice;
      stream >> vertice[0] >> vertice[1] >> vertice[2];
      m_vertices.push_back(vertice);
    }
    else if (str == "f")
    {
      array<uint32, 3> face;
      stream >> face[0];
      stream.ignore(numeric_limits<streamsize>::max(), ' ');
      stream >> face[1];
      stream.ignore(numeric_limits<streamsize>::max(), ' ');
      stream >> face[2];
      // in waveform obj vertice indexes are starting at 1
      --face[0];
      --face[1];
      --face[2];
      m_faces.push_back(face);
    }
  }

  m_vertices.shrink_to_fit();
  m_faces.shrink_to_fit();
}
