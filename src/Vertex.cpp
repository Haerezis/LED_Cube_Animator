#include "Vertex.hpp"
#include <cmath>

Vertex::Vertex() : Vertex(0,0,0)
{}

Vertex::Vertex(const Vertex& v, const bool normalize) : Vertex(v.x, v.y, v.z, normalize)
{}

Vertex::Vertex(const float _x, const float _y, const float _z, const bool normalize) : x(_x), y(_y), z(_z)
{
  if(normalize) this->normalize();
}

void Vertex::normalize()
{
  float norm = std::sqrt(x*x + y*y + z*z);
  x /= norm;
  y /= norm;
  z /= norm;
}

Vertex operator+(const Vertex& v1, const Vertex& v2)
{
  return Vertex(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vertex operator/(const Vertex& v, float n)
{
  return Vertex(v.x / n, v.y / n, v.z / n);
}

std::ostream& operator<<(std::ostream& out, const Vertex& v)
{
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return out;
}

namespace std
{
  size_t hash<Vertex>::operator()(const Vertex& v) const
  {
    size_t ret = 0;

    hash_combine(ret, v.x);
    hash_combine(ret, v.y);
    hash_combine(ret, v.z);

    return ret;
  }
}
