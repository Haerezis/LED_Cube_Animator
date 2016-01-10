#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include <vector>

#include "AnimationFrame.hpp"

class Animation
{
  protected:
    std::vector<AnimationFrame> _frames;
};

#endif
