#ifndef _LED_OFF_OPENGL_HPP
#define _LED_OFF_OPENGL_HPP

#include "LedOpenGL.hpp"

class LedOffOpenGL : public LedOpenGL
{
  public:
    LedOffOpenGL();
   
    ~LedOffOpenGL();
  
    void initialize(
        std::shared_ptr<GLfloat>& vertexBufferData,
        unsigned int vertexCount);

  protected:
    GLuint _loadShaders();
    
    std::unique_ptr<GLfloat, std::default_delete<GLfloat>> _vertexColorBufferData;
    
    GLuint _colorBuffer;
    GLuint _vertexColorID;
};

#endif
