#include "AnimationFrame.hpp"

AnimationFrame::AnimationFrame(unsigned int ledCubeSize)
{
  if(ledCubeSize == 0) ledCubeSize = 1;

  _cubeData.resize(ledCubeSize);
  for(auto &floor : _cubeData)
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
  if(floor >= _cubeData.size()) floor = _cubeData.size()-1;
  if(line >= _cubeData[floor].size()) line = _cubeData.size()-1;
  if(column >= _cubeData[floor][line].size()) column = _cubeData.size()-1;

  return _cubeData[floor][line][column];
}


void AnimationFrame::set(unsigned int floor, unsigned int line, unsigned int column, AnimationFrame::LEDState state)
{
  if(floor >= _cubeData.size()) floor = _cubeData.size()-1;
  if(line >= _cubeData[floor].size()) line = _cubeData.size()-1;
  if(column >= _cubeData[floor][line].size()) column = _cubeData.size()-1;

  _cubeData[floor][line][column] = state;
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
  const Value &floors = object["cube_data"];
  for (auto it_f = floors.Begin(); it_f != floors.End(); ++it_f, ++f)
    for (auto it_l = it_f->Begin(); it_l != it_f->End(); ++it_l, ++l)
      for (auto it_c = it_l->Begin(); it_c != it_l->End(); ++it_c, ++c)
        set(f, l , c, IntToEnum(it_c->GetUint()));
}

rapidjson::Value AnimationFrame::saveToJSON(rapidjson::Document::AllocatorType &allocator)
{
  using namespace rapidjson;
  Value object(Type::kObjectType);
  Value cube_data(Type::kArrayType);
  Value cube_data_floor(Type::kArrayType);
  Value cube_data_line(Type::kArrayType);
  
  for(auto &floor : _cubeData) {
    cube_data_floor = Value(Type::kArrayType);
    for(auto &line : floor) {
      cube_data_line = Value(Type::kArrayType);
      for(auto &elt : line){
        cube_data_line.PushBack((int)elt, allocator); 
      }
      cube_data_floor.PushBack(cube_data_line, allocator);
    }
    cube_data.PushBack(cube_data_floor, allocator);
  }
  object.AddMember("duration", _duration, allocator);
  object.AddMember("cube_data", cube_data, allocator);

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
