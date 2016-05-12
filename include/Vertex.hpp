#ifndef __VERTEX_HPP
#define __VERTEX_HPP

#include <ostream>
#include "utils.hpp"

struct Vertex
{
  float x;
  float y;
  float z;

  Vertex();
  Vertex(const float x, const float y, const float z, const bool normalize = false);
  Vertex(const Vertex& v, const bool normalize = false);
  void normalize();
  friend Vertex operator+(const Vertex& v1, const Vertex& v2);
  friend Vertex operator/(const Vertex& v1, float n);
  friend std::ostream& operator<<(std::ostream& out, const Vertex& v);
};

Vertex operator+(const Vertex& v1, const Vertex& v2);
Vertex operator/(const Vertex& v, float n);
std::ostream& operator<<(std::ostream& out, const Vertex& v);

namespace std {
template<>
class hash<Vertex>
{
  public:
    size_t operator()(const Vertex& v) const;
};
}

#endif
