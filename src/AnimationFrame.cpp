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


void AnimationFrame::load(std::istream &stream)
{

}


void AnimationFrame::save(std::ostream &stream)
{

}
