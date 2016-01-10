#include "Animation.hpp"

#include "rapidjson/document.h"


void Animation::save(std::ostream &stream)
{
  using namespace rapidjson;
  Document document;


}

void Animation::load(std::istream &stream)
{
  using namespace rapidjson;
  Document document;
  std::string s(std::istreambuf_iterator<char>(stream), {});
  document.Parse(s.c_str());
}
