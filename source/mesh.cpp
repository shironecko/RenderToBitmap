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
  return (uint32)m_vertices.size();
}

uint32 mesh::numFaces()
{
  return (uint32)m_faces.size();
}

vec3f mesh::vertice(uint32 index)
{
  return m_vertices[index];
}

vec3<meshFace> mesh::face(uint32 index)
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
      vec3f vertice;
      stream >> vertice[0] >> vertice[1] >> vertice[2];
      m_vertices.push_back(vertice);
    }
    else if (str == "f")
    {
      vec3<meshFace> face;
      for (uint32 i = 0; i < 3; ++i)
      {
        stream >> face[i].v;
        stream.ignore(numeric_limits<streamsize>::max(), '/');
        stream >> face[i].uv;
        stream.ignore(numeric_limits<streamsize>::max(), '/');
        stream >> face[i].n;

        // in waveform obj vertice indexes are starting at 1
        --face[i].v;
        --face[i].uv;
        --face[i].n;
      }

      m_faces.push_back(face);
    }
  }

  m_vertices.shrink_to_fit();
  m_faces.shrink_to_fit();
}
