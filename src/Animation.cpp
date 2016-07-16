#include "Animation.hpp"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


Animation::Animation(std::istream &stream)
{
  load(stream);
}

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
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  Value frames(kArrayType);

  auto &allocator = document.GetAllocator();
  document.SetObject();
  document.AddMember("cube_size", _cubeSize, allocator);
  for(auto &frame : _frames)
  {
    frames.PushBack(frame->saveToJSON(allocator), allocator);
  }
  document.AddMember("frames", frames, allocator);

  document.Accept(writer);
  stream << buffer.GetString();
}

void Animation::load(std::istream &stream)
{
  using namespace rapidjson;
  Document document;
  std::string s(std::istreambuf_iterator<char>(stream), {});
  document.Parse(s.c_str());

  if(!document["cube_size"].IsNumber() || !document["frames"].IsArray())
  {
    std::cerr << "[ERROR] File format isn't correct" << std::endl;
    return;
  }

  _frames.clear();
  _cubeSize = document["cube_size"].GetUint();
  const Value &array = document["frames"];
  for (auto it = array.Begin(); it != array.End(); ++it)
    _frames.emplace_back(new AnimationFrame(_cubeSize, _cubeSize, *it));
}

std::vector<std::shared_ptr<AnimationFrame>>& Animation::frames()
{
  return _frames;
}


void Animation::generate(std::ostream &stream)
{
  stream << "#ifndef __CUBE_DATA_HPP" << std::endl;
  stream << "#define __CUBE_DATA_HPP" << std::endl << std::endl;
  
  stream << "#include <stdint.h>" << std::endl << std::endl;
  //Data size : 8, 16, 32, 64
  //TODO

  stream << "#endif" << std::endl;
}
