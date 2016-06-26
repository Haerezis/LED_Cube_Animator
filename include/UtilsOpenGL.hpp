#ifndef _UTILS_OPENGL__HPP
#define _UTILS_OPENGL__HPP

#include <istream>
#include <memory>
#include <thread>
#include <mutex>

#include <QOpenGLFunctions>

class UtilsOpenGL : public QOpenGLFunctions
{
  public :
    static GLuint loadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream);
    static UtilsOpenGL& instance();

  protected:
    UtilsOpenGL();
    GLuint _loadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream);

    static std::unique_ptr<UtilsOpenGL> _instance;
    static std::once_flag _once_flag;
};

#endif
