#include <fstream>
#include <iostream>

#include "LedMonocolorOpenGL.hpp"
#include "UtilsOpenGL.hpp"

LedMonocolorOpenGL::LedMonocolorOpenGL() : LedOpenGL() {}

LedMonocolorOpenGL::~LedMonocolorOpenGL() {}

void LedMonocolorOpenGL::initialize(
    std::shared_ptr<GLfloat>& vertexBufferData,
    unsigned int vertexCount)
{
  LedOpenGL::initialize(vertexBufferData, vertexCount);

  _vertexColorID = glGetUniformLocation(_programID, "vertexColor");
}

void LedMonocolorOpenGL::prepareDraw(
    const QMatrix4x4& mvp,
    const QVector3D& color)
{
  static GLfloat colorf[3];
  
  LedOpenGL::prepareDraw(mvp);

  colorf[0] = color.x();
  colorf[1] = color.y();
  colorf[2] = color.z();
  glUniform3fv(_vertexColorID, 1, colorf);
}

GLuint LedMonocolorOpenGL::_loadShaders()
{
  std::ifstream vertexShaderFile, fragmentShaderFile;
  vertexShaderFile.open("shaders/LedMonocolorVertexShader.vertexshader");
  fragmentShaderFile.open("shaders/LedMonocolorFragmentShader.fragmentshader");

  return UtilsOpenGL::loadShaders(vertexShaderFile, fragmentShaderFile);
}
