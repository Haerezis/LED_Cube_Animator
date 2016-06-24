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
    
    static const float RotationAngleTick;

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    
    void loadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream);

    void setLedOnMode();
    void setLedOffMode();

    void paintLED(const QVector3D& translation = QVector3D(0,0,0));
    void paintLEDCube();

    void keyPressEvent(QKeyEvent * event);

  protected:
    GLuint _programID;
    GLuint _vertexPositionID;
    GLuint _vertexColorID;
    GLuint _mvpMatrixID;

    GLuint _vertexBuffer;
    GLuint _colorBuffer;

    std::unique_ptr<GLfloat, std::default_delete<GLfloat>> _vertexBufferData;
    std::unique_ptr<GLfloat, std::default_delete<GLfloat>> _vertexColorBufferData;
    unsigned int _vertexCount;
    float _distanceBetweenLED;

    QMatrix4x4 _cubeRotationMatrix;
    QMatrix4x4 _modelMatrix;
    QMatrix4x4 _viewMatrix;
    QMatrix4x4 _projectionMatrix;
    
    static GLfloat _vertex_buffer_data[];

    AnimationFrame* _currentFrame;
    QPoint _lastMousePosition;
    bool _mousePressed;

    static const float _nearPlaneDepth;
    static const float _farPlaneDepth;
};

#endif // CUBEOPENGL_H
