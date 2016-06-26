#ifndef _LED_OPENGL_HPP
#define _LED_OPENGL_HPP

#include <memory>

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>


class LedOpenGL  : public QOpenGLFunctions
{
  //Public functions
  public:
    LedOpenGL();
   
    virtual ~LedOpenGL();
    
    void initializeGL();
    virtual void initialize(
        std::shared_ptr<GLfloat>& vertexBufferData,
        unsigned int vertexCount);

    virtual void draw(const QMatrix4x4& mvp);

  //Protected functions
  protected:
    virtual GLuint _loadShaders() = 0;

  //Protected attributes
  protected:
    QOpenGLVertexArrayObject _vao;
    GLuint _programID;
    GLuint _vertexPositionID;
    GLuint _mvpMatrixID;

    GLuint _vertexBufferID;

    std::shared_ptr<GLfloat> _vertexBufferData;
    unsigned int _vertexCount;
};

#endif
