#ifndef CUBEOPENGL_H
#define CUBEOPENGL_H

#include <istream>
#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector3D>

#include "AnimationFrame.hpp"

class CubeOpenGL : public QOpenGLWidget, public QOpenGLFunctions
{
  public:
    CubeOpenGL(QWidget * parent = 0, Qt::WindowFlags f = 0);

    void setAnimationFrame(AnimationFrame* frame);

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void paintGL_();
    virtual void resizeGL(int w, int h);
    
    void LoadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream);

    void paintLED(const QVector3D& translation = QVector3D(0,0,0));
    void paintLEDCube();


  protected:
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

    AnimationFrame* _currentFrame;
};

#endif // CUBEOPENGL_H
