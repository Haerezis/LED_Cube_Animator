#include <iostream>
#include <fstream>
#include <cstdlib>

#include "CubeOpenGL.hpp"
#include "SphereVertices.hpp"

GLfloat CubeOpenGL::g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

void load(std::unique_ptr<GLfloat>& dst, unsigned int& dstCount)
{
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

  dstCount = sizeof(g_vertex_buffer_data) / sizeof(float);
  dst.reset(new GLfloat[dstCount]);

  for(unsigned int i = 0; i<dstCount ; i++)
    dst.get()[i] = g_vertex_buffer_data[i];
}

CubeOpenGL::CubeOpenGL(QWidget * parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
  SphereVertices sphere;
  //sphere.normalize(3);
  sphere.getFacesVertices(vertexBufferData, vertexCount);
  //load(vertexBufferData, vertexCount);

  vertexColorBufferData.reset(new GLfloat[vertexCount]);
  for(unsigned int i = 0 ; i < (vertexCount / 3) ; i++)
  {
    vertexColorBufferData.get()[i*3 + 0] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    vertexColorBufferData.get()[i*3 + 1] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    vertexColorBufferData.get()[i*3 + 2] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
  }

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
  //vertexShaderFile.open("shaders/SimpleVertexShader.vertexshader");
  //fragmentShaderFile.open("shaders/SimpleFragmentShader.fragmentshader");
  vertexShaderFile.open("shaders/TransformVertexShader.vertexshader");
  fragmentShaderFile.open("shaders/ColorFragmentShader.fragmentshader");
  if(vertexShaderFile.good() && fragmentShaderFile.good())
  {
    LoadShaders(vertexShaderFile, fragmentShaderFile);
  }
  else
  {
    std::cerr << "Error when trying to open shaders file : " << vertexShaderFile.good() << "," << fragmentShaderFile << std::endl;
    return;
  }

	// Get a handle for our buffers
	vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexColorID = glGetAttribLocation(programID, "vertexColor");
	mvpID = glGetUniformLocation(programID, "MVP");

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertexBufferData.get(), GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertexColorBufferData.get(), GL_STATIC_DRAW);
}

void CubeOpenGL::paintGL_()
{
  // Clear the screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Use our shader
  glUseProgram(programID);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(vertexPosition_modelspaceID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(
    vertexPosition_modelspaceID, // The attribute we want to configure
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(vertexPosition_modelspaceID);
}


void CubeOpenGL::paintGL()
{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, mvp.data());

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID, // The attribute we want to configure
			3,                           // size
			GL_FLOAT,                    // type
			GL_FALSE,                    // normalized?
			0,                           // stride
			(void*)0                     // array buffer offset
		);

		// 2nd attribute buffer : colors
    glEnableVertexAttribArray(vertexColorID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(
      vertexColorID,               // The attribute we want to configure
      3,                           // size
      GL_FLOAT,                    // type
      GL_FALSE,                    // normalized?
      0,                           // stride
      (void*)0                     // array buffer offset
    );

		// Draw the triangleS !
		glDrawArrays(GL_TRIANGLES, 0, vertexCount/3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexColorID);
}

void CubeOpenGL::resizeGL(int w, int h)
{
	QMatrix4x4 projection, view, model;
  projection.perspective(45.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);
	view.lookAt(
								QVector3D(4,3,-3), // Camera is at (4,3,-3), in World Space
								QVector3D(0,0,0), // and looks at the origin
								QVector3D(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	model.setToIdentity();
	mvp = projection * view * model;
}


void CubeOpenGL::LoadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream){

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
  programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
    std::cerr << "[ERROR] Program linking failure : " << &programErrorMessage[0] << std::endl;
	}
	
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
