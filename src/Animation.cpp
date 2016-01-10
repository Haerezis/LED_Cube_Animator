#include "Animation.hpp"

#include "rapidjson/document.h"



Animation::Animation(unsigned int cubeSize)
{
  this->cubeSize(cubeSize);
}

unsigned int Animation::cubeSize()
{
  return _cubeSize;
}

void Animation::cubeSize(unsigned int value)
{
  if(value == 0) value = 1;
  _cubeSize = value;
}


void Animation::save(std::ostream &stream)
{
  using namespace rapidjson;
  Document document;

  //TODO

}

void Animation::load(std::istream &stream)
{
  using namespace rapidjson;
  Document document;
  std::string s(std::istreambuf_iterator<char>(stream), {});
  document.Parse(s.c_str());
  //TODO
}


void generate(std::ostream &stream)
{
  //TODO
}
