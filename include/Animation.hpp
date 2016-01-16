#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include <vector>
#include <iostream>

#include "AnimationFrame.hpp"

class Animation
{
  public:
    enum DataFormat
    {
      Binary,
      Hexadecimal
    };

  public:
    Animation(unsigned int cubeSize);
    Animation(std::istream &stream);

    unsigned int cubeSize();
    void cubeSize(unsigned int cubeSize);

    std::vector<AnimationFrame>& frames();

    void save(std::ostream &stream);
    void load(std::istream &stream);
    void generate(std::ostream &stream, DataFormat format, unsigned int dataSize);
  protected:
    unsigned int _cubeSize;
    std::vector<AnimationFrame> _frames;
};

#endif
