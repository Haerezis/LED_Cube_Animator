#ifndef _LED_OPENGL_HPP
#define _LED_OPENGL_HPP

#include <memory>

#include <QOpenGLFunctions>
#include <QMatrix4x4>


class LedOpenGL  : public QOpenGLFunctions
{
  //Public functions
  public:
    LedOpenGL();
   
    virtual ~LedOpenGL();
    
    virtual void initialize(
        std::shared_ptr<GLfloat>& vertexBufferData,
        unsigned int vertexCount);

    void useProgram();
    
    virtual void paintLed(const QMatrix4x4& mvp);

  //Protected functions
  protected:
    virtual GLuint _loadShaders() = 0;

    virtual void _uninitialize();

  //Protected attributes
  protected:
    bool _initialized;

    GLuint _programID;
    GLuint _vertexPositionID;
    GLuint _mvpMatrixID;

    GLuint _vertexBufferID;

    std::shared_ptr<GLfloat> _vertexBufferData;
    unsigned int _vertexCount;
};

#endif
