#include "UtilsOpenGL.hpp"

#include <iostream>

std::unique_ptr<UtilsOpenGL> UtilsOpenGL::_instance;
std::once_flag UtilsOpenGL::_once_flag;

UtilsOpenGL::UtilsOpenGL()
{
  initializeOpenGLFunctions();
}

UtilsOpenGL& UtilsOpenGL::instance()
{
  std::call_once(_once_flag, []()
      {
        _instance.reset(new UtilsOpenGL());
      });
  return *_instance;
}

GLuint UtilsOpenGL::loadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream)
{
  return instance()._loadShaders(vertexShaderStream, fragmentShaderStream);
}

GLuint UtilsOpenGL::_loadShaders(std::istream& vertexShaderStream, std::istream& fragmentShaderStream)
{
  GLuint programID;
	GLint result = GL_FALSE;
	int infoLogLength;

	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the vertex Shader code from the file
  std::string vertexShaderSource;
	if(vertexShaderStream.good())
    vertexShaderSource.assign(std::istreambuf_iterator<char>(vertexShaderStream), {});
  else
    throw std::ios_base::failure("[ERROR] vertex shader stream is not valid");

	// Read the fragment Shader code from the file
  std::string fragmentShaderSource;
	if(fragmentShaderStream.good())
    fragmentShaderSource.assign(std::istreambuf_iterator<char>(fragmentShaderStream), {});
  else
    throw std::ios_base::failure("[ERROR] fragment shader stream is not valid");

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

  return programID;
}
