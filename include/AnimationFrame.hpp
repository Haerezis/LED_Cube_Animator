#ifndef __ANIMATION_FRAME_HPP
#define __ANIMATION_FRAME_HPP

#include <vector>
#include <iostream>

#include "rapidjson/document.h"

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
    unsigned int duration();
    void duration(unsigned int value);
    void loadFromJSON(const rapidjson::Value &object);
    rapidjson::Value saveToJSON(rapidjson::Document::AllocatorType &allocator);

  protected:
    unsigned int _duration;
    std::vector<std::vector<std::vector<LEDState>>> _ledCube;
};

#endif
