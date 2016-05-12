#ifndef CUBEOPENGL_H
#define CUBEOPENGL_H

#include <istream>
#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

class CubeOpenGL : public QOpenGLWidget, public QOpenGLFunctions
{
  public:
    CubeOpenGL(QWidget * parent = 0, Qt::WindowFlags f = 0);
  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void paintGL_();
    virtual void resizeGL(int w, int h);
    void LoadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream);

    GLuint programID;
    GLuint vertexPosition_modelspaceID;
    GLuint vertexColorID;
    GLuint mvpID;

    GLuint vertexBuffer;
    GLuint colorBuffer;

    std::unique_ptr<GLfloat, std::default_delete<GLfloat>> vertexBufferData;
    std::unique_ptr<GLfloat, std::default_delete<GLfloat>> vertexColorBufferData;
    unsigned int vertexCount;

    QMatrix4x4 mvp;
    
    static GLfloat g_vertex_buffer_data[];
};

#endif // CUBEOPENGL_H
