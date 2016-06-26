#include "LedOpenGL.hpp"
#include <UtilsOpenGL.hpp>
#include <iostream>

LedOpenGL::LedOpenGL() : QOpenGLFunctions()
{
}


LedOpenGL::~LedOpenGL()
{
}

void LedOpenGL::initializeGL()
{
  initializeOpenGLFunctions();
}

void LedOpenGL::initialize(
    std::shared_ptr<GLfloat>& vertexBufferData,
    unsigned int vertexCount)
{
  _vertexBufferData = vertexBufferData;
  _vertexCount = vertexCount;

  _programID = _loadShaders();

	_vertexPositionID = glGetAttribLocation(_programID, "vertexPosition");
	_mvpMatrixID = glGetUniformLocation(_programID, "MVP");

  _vao.create();
  _vao.bind();

  //Initialize and configure vertex position buffer/Attrib of the model
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexCount, _vertexBufferData.get(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(_vertexPositionID);
  glVertexAttribPointer(
    _vertexPositionID, // The attribute we want to configure
    3,                           // size
    GL_FLOAT,                    // type
    GL_FALSE,                    // normalized?
    0,                           // stride
    (void*)0                     // array buffer offset
  );

  _vao.release();
}


void LedOpenGL::draw(const QMatrix4x4& mvp)
{
  glUseProgram(_programID);
	// Get a handle for our buffers
  
  //Send the MVP matrix to the program
  glUniformMatrix4fv(_mvpMatrixID, 1, GL_FALSE, mvp.data());

  _vao.bind();
  
  //Draw model
  glDrawArrays(GL_TRIANGLES, 0, _vertexCount/3);

  _vao.release();
}
