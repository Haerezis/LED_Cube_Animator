#ifndef __SPHERE_VERTICES__HPP
#define __SPHERE_VERTICES__HPP

#include <vector>
#include <memory>
#include <unordered_set>
#include <ostream>

#include "utils.hpp"
#include "Vertex.hpp"

typedef std::tuple<Vertex, Vertex, Vertex> TriangleFace;

class SphereVertices
{
  public:
    SphereVertices();

    void normalize(unsigned int n);
    void getFacesVertices(std::unique_ptr<float>& dst, unsigned int& dstCount);
    void reset();

    friend std::ostream& operator<<(std::ostream& out, const SphereVertices& sv);

  protected:
    std::vector<Vertex> _icosahedronVertices;
    std::vector<TriangleFace> _sphereFaces;
};

std::ostream& operator<<(std::ostream& out, const SphereVertices& sv);


#endif
