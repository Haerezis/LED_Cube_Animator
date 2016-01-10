#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include <vector>
#include <iostream>

#include "AnimationFrame.hpp"

class Animation
{
  public:
    void save(std::ostream &stream);
    void load(std::istream &stream);
  protected:
    std::vector<AnimationFrame> _frames;
};

#endif
