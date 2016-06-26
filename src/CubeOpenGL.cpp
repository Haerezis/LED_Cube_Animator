#include <iostream>
#include <fstream>
#include <cstdlib>

#include <QMouseEvent>
#include <QPoint>
#include <QQuaternion>
#include "CubeOpenGL.hpp"
#include "SphereVertices.hpp"

const float CubeOpenGL::RotationAngleTick = 15.0f;
const float CubeOpenGL::_NearPlaneDepth = 0.1f;
const float CubeOpenGL::_FarPlaneDepth = 100.0f;
const float CubeOpenGL::_DistanceBetweenLED = 5.0f;

CubeOpenGL::CubeOpenGL(QWidget * parent, Qt::WindowFlags f) :
  QOpenGLWidget(parent, f),
  _mousePressed(false)
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

void CubeOpenGL::setAnimationFrame(AnimationFrame* frame)
{
  _currentFrame = frame;
}

void CubeOpenGL::initializeGL()
{
  initializeOpenGLFunctions();
	
  // Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

  //Initialize the QOpenGLFunctions of the LedOpenGLs
  _ledOffOpengl.initializeGL();
  _ledOnOpengl.initializeGL();

  //Initialize the data of the LedOpenGLs
  _ledOffOpengl.initialize(_vertexBufferData, _vertexCount);
  _ledOnOpengl.initialize(_vertexBufferData, _vertexCount);
	
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
}


void CubeOpenGL::paintLEDCube()
{
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

    for(unsigned int floor = 0 ; floor < cubeSize ; floor++)
    {
      for(unsigned int line = 0 ; line < cubeSize ; line++)
      {
        for(unsigned int column = 0 ; column < cubeSize ; column++)
        {
          QVector3D ledPosition = cubeOriginOffset + QVector3D(floor, column, line) * distanceLedVector;
          
          if(_currentFrame->get(floor, line, column) == AnimationFrame::LEDState::On)
          {
            _ledOnOpengl.draw(_getMVP(ledPosition));
          }
          else if(_currentFrame->get(floor, line, column) == AnimationFrame::LEDState::Off)
          {
            _ledOffOpengl.draw(_getMVP(ledPosition));
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
