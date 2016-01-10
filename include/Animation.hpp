#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include <vector>
#include <iostream>

#include "AnimationFrame.hpp"

class Animation
{
  public:
    Animation(unsigned int cubeSize);

    unsigned int cubeSize();
    void cubeSize(unsigned int cubeSize);


    void save(std::ostream &stream);
    void load(std::istream &stream);
    void generate(std::ostream &stream);
  protected:
    unsigned int _cubeSize;
    std::vector<AnimationFrame> _frames;
};

#endif
