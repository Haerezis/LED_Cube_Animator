#include "LedOpenGL.hpp"
#include <UtilsOpenGL.hpp>

LedOpenGL::LedOpenGL() : _initialized(false) {}


LedOpenGL::~LedOpenGL()
{
  _uninitialize();
}


void LedOpenGL::initialize(
    std::shared_ptr<GLfloat>& vertexBufferData,
    unsigned int vertexCount)
{
  _vertexBufferData = vertexBufferData;
  _vertexCount = vertexCount;

  _programID = _loadShaders();

	// Get a handle for our buffers
	_vertexPositionID = glGetAttribLocation(_programID, "vertexPosition");
	_mvpMatrixID = glGetUniformLocation(_programID, "MVP");

  //Initialize and configure vertex position buffer/VAO/Attrib of the model
	glGenBuffers(1, &_vertexBufferID);
  glEnableVertexAttribArray(_vertexPositionID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexCount, _vertexBufferData.get(), GL_STATIC_DRAW);
  glVertexAttribPointer(
    _vertexPositionID, // The attribute we want to configure
    3,                           // size
    GL_FLOAT,                    // type
    GL_FALSE,                    // normalized?
    0,                           // stride
    (void*)0                     // array buffer offset
  );

  _initialized = true;
}


void LedOpenGL::useProgram()
{
  // Use our shader
  glUseProgram(_programID);
}


void LedOpenGL::paintLed(const QMatrix4x4& mvp)
{
  //Send the MVP matrix to the program
  glUniformMatrix4fv(_mvpMatrixID, 1, GL_FALSE, mvp.data());
  
  //Draw model
  glDrawArrays(GL_TRIANGLES, 0, _vertexCount/3);
}


void LedOpenGL::_uninitialize()
{
  if(_initialized)
  {
//TODO
  }
}
