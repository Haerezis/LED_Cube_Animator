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

AnimationFrame::AnimationFrame(unsigned int ledCubeSize, const rapidjson::Value &object) :
  AnimationFrame(ledCubeSize)
{
  loadFromJSON(object);
}

AnimationFrame::~AnimationFrame() {}

AnimationFrame::LEDState AnimationFrame::get(unsigned int floor, unsigned int line, unsigned int column)
{
  if(floor >= _ledCube.size()) floor = _ledCube.size()-1;
  if(line >= _ledCube[floor].size()) line = _ledCube.size()-1;
  if(column >= _ledCube[floor][line].size()) column = _ledCube.size()-1;

  return _ledCube[floor][line][column];
}


void AnimationFrame::set(unsigned int floor, unsigned int line, unsigned int column, AnimationFrame::LEDState state)
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
  if(value == 0) value = 1;
  _duration = value;
}


void AnimationFrame::loadFromJSON(const rapidjson::Value &object)
{
  using namespace rapidjson;
  unsigned int f = 0, l = 0, c = 0;

  object["duration"].GetUint();
  const Value &floors = object["frames"];
  for (auto it_f = floors.Begin(); it_f != floors.End(); ++it_f, ++f)
    for (auto it_l = it_f->Begin(); it_l != it_f->End(); ++it_l, ++l)
      for (auto it_c = it_l->Begin(); it_c != it_l->End(); ++it_c, ++c)
        set(f, l , c, IntToEnum(it_c->GetUint()));
}

rapidjson::Value AnimationFrame::saveToJSON(rapidjson::Document::AllocatorType &allocator)
{
  rapidjson::Value object(rapidjson::Type::kObjectType);
  object.AddMember("duration", _duration, allocator);
  //TODO

  return object;
}


AnimationFrame::LEDState AnimationFrame::IntToEnum(unsigned int value)
{
  AnimationFrame::LEDState res = Off;
  switch(value)
  {
    case 1:
      res = On;
      break;
    case 0:
    default:
      break;
  }
  
  return res;
}
