#ifndef __ANIMATION_FRAME_HPP
#define __ANIMATION_FRAME_HPP

#include <vector>
#include <iostream>

class AnimationFrame
{
  public:
    enum LEDState
    {
      Off,
      On
    };

    AnimationFrame(unsigned int ledCubeSize);
    virtual ~AnimationFrame();

    LEDState get(unsigned int floor, unsigned int line, unsigned int column);
    void get(unsigned int floor, unsigned int line, unsigned int column, LEDState state);
    void load(std::istream &stream);
    void save(std::ostream &stream);

  protected:
    std::vector<std::vector<std::vector<LEDState>>> _ledCube;
};

#endif
