#include <iostream>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include <QMouseEvent>
#include <QPoint>
#include <QVector4D>
#include <QQuaternion>
#include "CubeOpenGL.hpp"
#include "SphereVertices.hpp"

const float CubeOpenGL::RotationAngleTick = 15.0f;
const float CubeOpenGL::_NearPlaneDepth = 0.1f;
const float CubeOpenGL::_FarPlaneDepth = 100.0f;
const float CubeOpenGL::_DistanceBetweenLED = 5.0f;

CubeOpenGL::CubeOpenGL(QWidget * parent, Qt::WindowFlags f) :
  QOpenGLWidget(parent, f)
{
  float *vertexData;
  SphereVertices sphere;
  sphere.normalize(3);
  sphere.getFacesVertices(vertexData, _vertexCount);
  _vertexBufferData.reset(vertexData);

  this->setFocusPolicy(Qt::StrongFocus);
}

void CubeOpenGL::keyPressEvent(QKeyEvent * event)
{
  static const QVector3D xAxis(1.0f, 0.0f, 0.0f);
  static const QVector3D yAxis(0.0f, 1.0f, 0.0f);
  static const QVector3D zAxis(0.0f, 0.0f, 1.0f);

  switch(event->key())
  {
    case Qt::Key_2:
      _cubeRotationMatrix.rotate(CubeOpenGL::RotationAngleTick, xAxis);
      break;
    case Qt::Key_4:
      _cubeRotationMatrix.rotate(CubeOpenGL::RotationAngleTick, yAxis);
      break;
    case Qt::Key_6:
      _cubeRotationMatrix.rotate(-CubeOpenGL::RotationAngleTick, yAxis);
      break;
    case Qt::Key_8:
      _cubeRotationMatrix.rotate(-CubeOpenGL::RotationAngleTick, xAxis);
      break;
    case Qt::Key_1:
      _cubeRotationMatrix.setToIdentity();
      //Z top, X right
      _cubeRotationMatrix.rotate(-90.0f, xAxis);
      break;
    case Qt::Key_3:
      //Z top, Y right
      _cubeRotationMatrix.setToIdentity();
      _cubeRotationMatrix.rotate(-90.0f, zAxis);
      _cubeRotationMatrix.rotate(-90.0f, xAxis);
      break;
    case Qt::Key_7:
      //Y top, X right
      _cubeRotationMatrix.setToIdentity();
      break;
    default:
      break;
  }
  update();
}

void CubeOpenGL::mousePressEvent(QMouseEvent * event)
{
  int x = event->x();
  int y = _widgetHeight - event->y();
  GLubyte pos[4];
  
  makeCurrent();
  paintLEDCube(true);
  glFlush();
  glFinish();
  glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pos);
  doneCurrent();

  //(255,255,255) color is the background color (normally) and should be ignored
  if((pos[0] + pos[1] + pos[2]) < (255 + 255 + 255))
    _currentFrame->flip(pos[0], pos[1], pos[2]);

  update();
}

void CubeOpenGL::setAnimationFrame(AnimationFrame* frame)
{
  _currentFrame = frame;
}

void CubeOpenGL::initializeGL()
{
  initializeOpenGLFunctions();
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

  //Initialize the QOpenGLFunctions of the LedOpenGLs
  _ledOffOpengl.initializeGL();
  _ledOnOpengl.initializeGL();
  _ledMonocolorOpengl.initializeGL();

  //Initialize the data of the LedOpenGLs
  _ledOffOpengl.initialize(_vertexBufferData, _vertexCount);
  _ledOnOpengl.initialize(_vertexBufferData, _vertexCount);
  _ledMonocolorOpengl.initialize(_vertexBufferData, _vertexCount);
	
  _viewMatrix.lookAt(
								QVector3D(0,0,-25), // Camera is at (4,3,-3), in World Space
								QVector3D(0,0,0), // and looks at the origin
								QVector3D(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
  
  _modelMatrix.scale(0.5f, 0.5f, 0.5f);
}


void CubeOpenGL::paintGL()
{
  paintLEDCube();
}

void CubeOpenGL::resizeGL(int w, int h)
{
  _projectionMatrix.setToIdentity();
  _projectionMatrix.perspective(15.0f, (static_cast<float>(w) / static_cast<float>(h)), _NearPlaneDepth, _FarPlaneDepth);
  _widgetWidth = w;
  _widgetHeight = h;
}


QVector3D CubeOpenGL::_getMouseRayCast(unsigned int x, unsigned int y)
{
  QVector3D ray_nds;
  ray_nds.setX((2.0f * static_cast<float>(x)) / static_cast<float>(_widgetWidth) - 1.0f);
  ray_nds.setY(1.0f - (2.0f * static_cast<float>(y)) / static_cast<float>(_widgetHeight));
  ray_nds.setZ(1.0f);

  QVector4D ray_clip;
  ray_clip.setX(ray_nds.x());
  ray_clip.setY(ray_nds.y());
  ray_clip.setZ(-1.0f);
  ray_clip.setW(1.0f);

  QVector4D ray_eye(_projectionMatrix.inverted() * ray_clip);
  ray_eye.setZ(-1.0f);
  ray_eye.setW(0.0f);
  
  QVector3D ray_wor(_viewMatrix.inverted() * ray_eye);
  ray_wor.normalize();

  return ray_wor;
}


void CubeOpenGL::paintLEDCube(bool pickingMode)
{
  if(pickingMode)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  else
    glClearColor(0.1f, 0.1f, 0.2f, 0.0f);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(_currentFrame != nullptr)
  {
    unsigned int cubeSize = _currentFrame->size();

    //DEBUG
    //cubeSize = 3;

    const float cubeDimension = (cubeSize-1) * _DistanceBetweenLED;
    const QVector3D cubeOriginOffset = QVector3D(-cubeDimension/2.0, -cubeDimension/2.0, -cubeDimension/2.0);
    const QVector3D distanceLedVector(_DistanceBetweenLED, _DistanceBetweenLED, _DistanceBetweenLED);

    QVector3D ledColor;
    QVector3D ledPosition;
    for(unsigned int floor = 0 ; floor < cubeSize ; floor++)
    {
      for(unsigned int line = 0 ; line < cubeSize ; line++)
      {
        for(unsigned int column = 0 ; column < cubeSize ; column++)
        {
          ledPosition = cubeOriginOffset + QVector3D(floor, column, line) * distanceLedVector;
          QMatrix4x4 mvp = _getMVP(ledPosition);

          if(pickingMode)
          {
            ledColor.setX(static_cast<float>(floor) / 255.0f);
            ledColor.setY(static_cast<float>(line) / 255.0f);
            ledColor.setZ(static_cast<float>(column) / 255.0f);

            //std::cout << ledColor.x() << " " << ledColor.y() << " " << ledColor.z() << std::endl;

            _ledMonocolorOpengl.prepareDraw(mvp, ledColor);
            _ledMonocolorOpengl.draw();
          }
          else
          {
            if(_currentFrame->get(floor, line, column) == AnimationFrame::LEDState::On)
            {
              _ledOnOpengl.prepareDraw(mvp);
              _ledOnOpengl.draw();
            }
            else if(_currentFrame->get(floor, line, column) == AnimationFrame::LEDState::Off)
            {
              _ledOffOpengl.prepareDraw(mvp);
              _ledOffOpengl.draw();
            }
            else
            {
              std::cout << "Unknown LED state" << std::endl;
              continue;
            }
          }
        }
      }
    }
  }
}

QMatrix4x4 CubeOpenGL::_getMVP(const QVector3D& modelTranslation)
{
  QMatrix4x4 mvp;

  mvp = _modelMatrix;
  mvp.translate(modelTranslation);
  mvp = _cubeRotationMatrix * mvp;
  mvp = _viewMatrix * mvp;
  mvp = _projectionMatrix * mvp;

  return mvp;
}
