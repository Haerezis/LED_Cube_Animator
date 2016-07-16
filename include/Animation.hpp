#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include <vector>
#include <iostream>
#include <memory>

#include "AnimationFrame.hpp"

class Animation
{
  public:
    Animation(unsigned int cubeSize);
    Animation(std::istream &stream);

    unsigned int cubeSize();
    void cubeSize(unsigned int cubeSize);

    std::vector<std::shared_ptr<AnimationFrame>>& frames();

    void save(std::ostream &stream);
    void load(std::istream &stream);
    void generate(std::ostream &stream);
  protected:
    unsigned int _cubeSize;
    std::vector<std::shared_ptr<AnimationFrame>> _frames;
};

#endif
