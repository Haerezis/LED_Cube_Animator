#include <cmath>
#include <iostream>
#include <iterator>

#include "SphereVertices.hpp"

SphereVertices::SphereVertices()
{
  float t = (1.0 + std::sqrt(5.0)) / 2.0;
  _icosahedronVertices.emplace_back(-1,  t, 0, true);
  _icosahedronVertices.emplace_back( 1,  t, 0, true);
  _icosahedronVertices.emplace_back(-1, -t, 0, true);
  _icosahedronVertices.emplace_back( 1, -t, 0, true);

  _icosahedronVertices.emplace_back( 0, -1,  t, true);
  _icosahedronVertices.emplace_back( 0,  1,  t, true);
  _icosahedronVertices.emplace_back( 0, -1, -t, true);
  _icosahedronVertices.emplace_back( 0,  1, -t, true);
  
  _icosahedronVertices.emplace_back( t, 0, -1, true);
  _icosahedronVertices.emplace_back( t, 0,  1, true);
  _icosahedronVertices.emplace_back(-t, 0, -1, true);
  _icosahedronVertices.emplace_back(-t, 0,  1, true);

  reset();
}

void SphereVertices::normalize(unsigned int n)
{
  while(n > 0)
  {
    Vertex v1,v2,v3;
    std::vector<TriangleFace> faces;
    for(const auto& face : _sphereFaces)
    {
      std::tie(v1,v2,v3) = face;

      Vertex v12((v1 + v2) / 2.0, true);
      Vertex v23((v2 + v3) / 2.0, true);
      Vertex v31((v3 + v1) / 2.0, true);

      faces.emplace_back(std::make_tuple(v1, v12, v31));
      faces.emplace_back(std::make_tuple(v2, v23, v12));
      faces.emplace_back(std::make_tuple(v3, v23, v31));
      faces.emplace_back(std::make_tuple(v12, v23, v31));
    }
    _sphereFaces = faces;

    --n;
  }
}

void SphereVertices::getFacesVertices(float*& dst, unsigned int& dstCount)
{
  dstCount = _sphereFaces.size() * 3 * 3;//Number of faces * Number of vertex per face * Number of coordinates in a Vertex.
  dst = new float[dstCount];

  unsigned int i = 0;
  for(const auto& face : _sphereFaces)
  {
    dst[i]   = std::get<0>(face).x;
    dst[i+1] = std::get<0>(face).y;
    dst[i+2] = std::get<0>(face).z;

    dst[i+3] = std::get<1>(face).x;
    dst[i+4] = std::get<1>(face).y;
    dst[i+5] = std::get<1>(face).z;

    dst[i+6] = std::get<2>(face).x;
    dst[i+7] = std::get<2>(face).y;
    dst[i+8] = std::get<2>(face).z;

    i+=9;
  }
}

std::ostream& operator<<(std::ostream& out, const SphereVertices& sv)
{
  unsigned int i = 0;
  Vertex v1,v2,v3;
  out << "Number of faces : " << sv._sphereFaces.size() << std::endl;
  for(const auto& face : sv._sphereFaces)
  {
    std::tie(v1,v2,v3) = face;
    out << i << " - " << v1 << " " << v2 << " " << v3 << std::endl;
    i++;
  }
  return out;
}

void SphereVertices::reset()
{
  _sphereFaces.clear();

  Vertex  v0(_icosahedronVertices[0]);
  Vertex  v1(_icosahedronVertices[1]);
  Vertex  v2(_icosahedronVertices[2]);
  Vertex  v3(_icosahedronVertices[3]);
  Vertex  v4(_icosahedronVertices[4]);
  Vertex  v5(_icosahedronVertices[5]);
  Vertex  v6(_icosahedronVertices[6]);
  Vertex  v7(_icosahedronVertices[7]);
  Vertex  v8(_icosahedronVertices[8]);
  Vertex  v9(_icosahedronVertices[9]);
  Vertex v10(_icosahedronVertices[10]);
  Vertex v11(_icosahedronVertices[11]);

  //5 faces around point 0
  _sphereFaces.emplace_back(std::make_tuple(v0, v11, v5));
  _sphereFaces.emplace_back(std::make_tuple(v0, v5, v1));
  _sphereFaces.emplace_back(std::make_tuple(v0, v1, v7));
  _sphereFaces.emplace_back(std::make_tuple(v0, v7, v10));
  _sphereFaces.emplace_back(std::make_tuple(v0, v10, v11));

  ////5 adjacent faces
  _sphereFaces.emplace_back(std::make_tuple(v1, v5, v9));
  _sphereFaces.emplace_back(std::make_tuple(v5, v11, v4));
  _sphereFaces.emplace_back(std::make_tuple(v11, v10, v2));
  _sphereFaces.emplace_back(std::make_tuple(v10, v7, v6));
  _sphereFaces.emplace_back(std::make_tuple(v7, v1, v8));

  ////5 faces around point 3
  _sphereFaces.emplace_back(std::make_tuple(v3, v9, v4));
  _sphereFaces.emplace_back(std::make_tuple(v3, v4, v2));
  _sphereFaces.emplace_back(std::make_tuple(v3, v2, v6));
  _sphereFaces.emplace_back(std::make_tuple(v3, v6, v8));
  _sphereFaces.emplace_back(std::make_tuple(v3, v8, v9));

  ////5 adjacent faces
  _sphereFaces.emplace_back(std::make_tuple(v4, v9, v5));
  _sphereFaces.emplace_back(std::make_tuple(v2, v4, v11));
  _sphereFaces.emplace_back(std::make_tuple(v6, v2, v10));
  _sphereFaces.emplace_back(std::make_tuple(v8, v6, v7));
  _sphereFaces.emplace_back(std::make_tuple(v9, v8, v1));
}
