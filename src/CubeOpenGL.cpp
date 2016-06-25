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
  SphereVertices sphere;
  sphere.normalize(3);
  sphere.getFacesVertices(_vertexBufferData, _vertexCount);

  _vertexColorBufferData.reset(new GLfloat[_vertexCount]);
  for(unsigned int i = 0 ; i < (_vertexCount / 3) ; i++)
  {
    _vertexColorBufferData.get()[i*3 + 0] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    _vertexColorBufferData.get()[i*3 + 1] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    _vertexColorBufferData.get()[i*3 + 2] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
  }

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
  std::ifstream vertexShaderFile, fragmentShaderFile;
  initializeOpenGLFunctions();
	
  // Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 


	// Create and compile our GLSL program from the shaders
  vertexShaderFile.open("shaders/TransformVertexShader.vertexshader");
  fragmentShaderFile.open("shaders/ColorFragmentShader.fragmentshader");
  if(vertexShaderFile.good() && fragmentShaderFile.good())
  {
    loadShaders(vertexShaderFile, fragmentShaderFile);
  }
  else
  {
    std::cerr << "Error when trying to open shaders file : " << vertexShaderFile.good() << "," << fragmentShaderFile << std::endl;
    return;
  }

	// Get a handle for our buffers
	_vertexPositionID = glGetAttribLocation(_programID, "vertexPosition");
	_vertexColorID = glGetAttribLocation(_programID, "vertexColor");
	_mvpMatrixID = glGetUniformLocation(_programID, "MVP");

  //Initialize and configure vertex position buffer/VAO/Attrib of the model
	glGenBuffers(1, &_vertexBuffer);
  glEnableVertexAttribArray(_vertexPositionID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexCount, _vertexBufferData.get(), GL_STATIC_DRAW);
  glVertexAttribPointer(
    _vertexPositionID, // The attribute we want to configure
    3,                           // size
    GL_FLOAT,                    // type
    GL_FALSE,                    // normalized?
    0,                           // stride
    (void*)0                     // array buffer offset
  );

  //Initialize and configure color buffer/VAO/Attrib
	glGenBuffers(1, &_colorBuffer);
  glEnableVertexAttribArray(_vertexColorID);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertexCount, _vertexColorBufferData.get(), GL_STATIC_DRAW);
  glVertexAttribPointer(
    _vertexColorID,               // The attribute we want to configure
    3,                           // size
    GL_FLOAT,                    // type
    GL_FALSE,                    // normalized?
    0,                           // stride
    (void*)0                     // array buffer offset
  );
	
  _viewMatrix.lookAt(
								QVector3D(0,0,-25), // Camera is at (4,3,-3), in World Space
								QVector3D(0,0,0), // and looks at the origin
								QVector3D(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
  
  _modelMatrix.scale(0.5f, 0.5f, 0.5f);

  glUseProgram(_programID);
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


void CubeOpenGL::loadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream){

	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the vertex Shader code from the file
  std::string vertexShaderSource;
	if(vertexShaderStream.good()) {
    vertexShaderSource.assign(std::istreambuf_iterator<char>(vertexShaderStream), {});
	}
  else {
    std::cout << "[ERROR] vertex shader stream is not valid" << std::endl;
		return;
	}

	// Read the fragment Shader code from the file
  std::string fragmentShaderSource;
	if(fragmentShaderStream.good()) {
    fragmentShaderSource.assign(std::istreambuf_iterator<char>(fragmentShaderStream), {});
	}
  else {
    std::cout << "[ERROR] fragment shader stream is not valid" << std::endl;
		return;
	}

	GLint result = GL_FALSE;
	int infoLogLength;


	// Compile vertex Shader
  std::cout << "Compiling Vertex Shader" << std::endl;
	const char * vertexSourcePointer = vertexShaderSource.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (result != GL_TRUE) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
    std::cerr << "[ERROR] Vertex Shader compilation failure : " << &vertexShaderErrorMessage[0] << std::endl;
	}

	// Compile fragment Shader
  std::cout << "Compiling Fragment Shader" << std::endl;
	const char * fragmentSourcePointer = fragmentShaderSource.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
    std::cerr << "[ERROR] Fragment Shader compilation failure : " << &fragmentShaderErrorMessage[0] << std::endl;
	}


	// Link the program
  std::cout << "Creating and linking program" << std::endl;
  _programID = glCreateProgram();
	glAttachShader(_programID, vertexShaderID);
	glAttachShader(_programID, fragmentShaderID);
	glLinkProgram(_programID);

	// Check the program
	glGetProgramiv(_programID, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(_programID, infoLogLength, NULL, &programErrorMessage[0]);
    std::cerr << "[ERROR] Program linking failure : " << &programErrorMessage[0] << std::endl;
	}
	
	glDetachShader(_programID, vertexShaderID);
	glDetachShader(_programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}


void CubeOpenGL::paintLED(const QVector3D& translation)
{
  QMatrix4x4 mvp;

  mvp = _modelMatrix;
  mvp.translate(translation);
  mvp = _cubeRotationMatrix * mvp;
  mvp = _viewMatrix * mvp;
  mvp = _projectionMatrix * mvp;

  glUniformMatrix4fv(_mvpMatrixID, 1, GL_FALSE, mvp.data());

  // Draw the triangleS !
  glDrawArrays(GL_TRIANGLES, 0, _vertexCount/3);
}


void CubeOpenGL::paintLEDCube()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(_currentFrame != nullptr)
  {
    unsigned int cubeSize = _currentFrame->size();

    //DEBUG
    cubeSize = 3;

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
            paintLED(ledPosition);
          }
          else if(_currentFrame->get(floor, line, column) == AnimationFrame::LEDState::Off)
          {
            paintLED(ledPosition);
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
