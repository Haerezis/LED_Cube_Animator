#ifndef _LED_MONONOCOLOR_OPENGL_HPP
#define _LED_MONONOCOLOR_OPENGL_HPP

#include "LedOpenGL.hpp"

class LedMonocolorOpenGL : public LedOpenGL
{
  public:
    LedMonocolorOpenGL();
   
    ~LedMonocolorOpenGL();
    
    void initialize(
        std::shared_ptr<GLfloat>& vertexBufferData,
        unsigned int vertexCount);
    
    void prepareDraw(
        const QMatrix4x4& mvp,
        const QVector3D& color);

  protected:
    GLuint _loadShaders();
    
    GLuint _vertexColorID;
};

#endif
