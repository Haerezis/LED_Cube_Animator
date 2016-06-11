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
    static LEDState IntToEnum(unsigned int value);

    AnimationFrame(unsigned int ledCubeSize);
    AnimationFrame(unsigned int ledCubeSize, const rapidjson::Value &object);
    ~AnimationFrame() {}

    LEDState get(unsigned int floor, unsigned int line, unsigned int column);
    void set(unsigned int floor, unsigned int line, unsigned int column, LEDState state);
    void clear();
    unsigned int duration();
    unsigned int size();
    void duration(unsigned int value);
    void loadFromJSON(const rapidjson::Value &object);
    rapidjson::Value saveToJSON(rapidjson::Document::AllocatorType &allocator);

  protected:
    unsigned int _duration;
    std::vector<std::vector<std::vector<LEDState>>> _cubeData;
};

#endif
