#include "AnimationFrame.hpp"

AnimationFrame::AnimationFrame(unsigned int ledCubeSize)
{
  if(ledCubeSize == 0) ledCubeSize = 1;

  _ledCube.resize(ledCubeSize);
  for(auto &floor : _ledCube)
  {
    floor.resize(ledCubeSize);
    for(auto &line : floor)
    {
      line.resize(ledCubeSize);
      for(auto &elt : line)
      {
        elt = LEDState::Off;
      }
    }
  }
}

AnimationFrame::~AnimationFrame()
{
}


AnimationFrame::LEDState AnimationFrame::get(unsigned int floor, unsigned int line, unsigned int column)
{
  if(floor >= _ledCube.size()) floor = _ledCube.size()-1;
  if(line >= _ledCube[floor].size()) line = _ledCube.size()-1;
  if(column >= _ledCube[floor][line].size()) column = _ledCube.size()-1;

  return _ledCube[floor][line][column];
}


void AnimationFrame::get(unsigned int floor, unsigned int line, unsigned int column, AnimationFrame::LEDState state)
{
  if(floor >= _ledCube.size()) floor = _ledCube.size()-1;
  if(line >= _ledCube[floor].size()) line = _ledCube.size()-1;
  if(column >= _ledCube[floor][line].size()) column = _ledCube.size()-1;

  _ledCube[floor][line][column] = state;
}


unsigned int AnimationFrame::duration()
{
  return _duration;
}
void AnimationFrame::duration(unsigned int value)
{
  _duration = value;
}


void AnimationFrame::loadFromJSON(const rapidjson::Value &object)
{
  //TODO
  object["duration"].GetUint();
}

rapidjson::Value AnimationFrame::saveToJSON(rapidjson::Document::AllocatorType &allocator)
{
  //TODO
  rapidjson::Value object(rapidjson::Type::kObjectType);
  object.AddMember("duration", _duration, allocator);

  return object;
}
