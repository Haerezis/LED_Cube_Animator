#include <fstream>

#include "LedOnOpenGL.hpp"
#include "UtilsOpenGL.hpp"

LedOnOpenGL::LedOnOpenGL() : LedOpenGL() {}

LedOnOpenGL::~LedOnOpenGL() {}

void LedOnOpenGL::initialize(
    std::shared_ptr<GLfloat>& vertexBufferData,
    unsigned int vertexCount)
{
  LedOpenGL::initialize(vertexBufferData, vertexCount);

  _vertexColorBufferData.reset(new GLfloat[_vertexCount]);
  for(unsigned int i = 0 ; i < (_vertexCount / 3) ; i++)
  {
    _vertexColorBufferData.get()[i*3 + 0] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    _vertexColorBufferData.get()[i*3 + 1] = 1.0f;
    _vertexColorBufferData.get()[i*3 + 2] = 1.0f;
  }

  //Initialize and configure color buffer/VAO/Attrib
	glGenBuffers(1, &_colorBuffer);
	_vertexColorID = glGetAttribLocation(_programID, "vertexColor");
  glEnableVertexAttribArray(_vertexColorID);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertexCount, _vertexColorBufferData.get(), GL_STATIC_DRAW);
  glVertexAttribPointer(
    _vertexColorID,               // The attribute we want to configure
    3,                           // size
    GL_FLOAT,                    // type
    GL_FALSE,                    // normalized?
    0,                           // stride
    (void*)0                     // array buffer offset
  );
}

GLuint LedOnOpenGL::_loadShaders()
{
  std::ifstream vertexShaderFile, fragmentShaderFile;
  vertexShaderFile.open("shaders/LedOnVertexShader.vertexshader");
  fragmentShaderFile.open("shaders/LedOnFragmentShader.fragmentshader");

  return UtilsOpenGL::loadShaders(vertexShaderFile, fragmentShaderFile);
}
