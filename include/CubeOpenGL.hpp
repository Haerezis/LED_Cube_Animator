#ifndef CUBEOPENGL_H
#define CUBEOPENGL_H

#include <istream>
#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector3D>

#include "AnimationFrame.hpp"
#include "LedOffOpenGL.hpp"
#include "LedOnOpenGL.hpp"

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
    
    void paintLEDCube();

    QMatrix4x4 _getMVP(const QVector3D& modelTranslation = QVector3D(0,0,0));

    void keyPressEvent(QKeyEvent * event);

  protected:
    LedOffOpenGL _ledOffOpengl;
    LedOnOpenGL _ledOnOpengl;

    std::shared_ptr<GLfloat> _vertexBufferData;
    unsigned int _vertexCount;

    QMatrix4x4 _cubeRotationMatrix;
    QMatrix4x4 _modelMatrix;
    QMatrix4x4 _viewMatrix;
    QMatrix4x4 _projectionMatrix;
    
    AnimationFrame* _currentFrame;
    QPoint _lastMousePosition;
    bool _mousePressed;

    static const float _NearPlaneDepth;
    static const float _FarPlaneDepth;
    static const float _DistanceBetweenLED;
};

#endif // CUBEOPENGL_H
