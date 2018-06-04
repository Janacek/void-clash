/********************************************************************
**
**              Shader.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "glm/gtc/type_ptr.hpp"
#include "Shader.hpp"
#include "CheckError.hpp"
#include "Log.hpp"

using namespace ogl;

static GLuint currentProgram = 0;

/////////////////////////////////////////////////////////////////////
////	Constructor / Desctructor
/////////////////////////////////////////////////////////////////////

Shader::Shader(void)
{
	_program = 0;
	_vertex = 0;
	_fragment = 0;
	_geometry = 0;
}

Shader::~Shader(void)
{
}

void Shader::destroy(void)
{
	if (_vertex != 0)
		glDeleteShader(_vertex);
	if (_fragment != 0)
		glDeleteShader(_fragment);
	if (_geometry != 0)
		glDeleteShader(_geometry);
	if (_program != 0)
		glDeleteProgram(_program);

	currentProgram = 0;

	checkError();
}

/////////////////////////////////////////////////////////////////////
////	Load a shader from a file
/////////////////////////////////////////////////////////////////////

void Shader::loadFromFile(const std::string &vertexSRC,
	const std::string &fragmentSRC,
	const std::string &geometrySRC)
{
	VC_INFO("Shader::Loading " + vertexSRC + ", " + fragmentSRC + ", " + geometrySRC);

	_program = glCreateProgram();
	if (_program == INVALID_PROGRAM_ID)
		VC_ERROR("Shader::unable to create shader program");

	checkError();

	if (vertexSRC.size())
	{
		_vertex = glCreateShader(GL_VERTEX_SHADER);
		if (_vertex == INVALID_PROGRAM_ID)
			VC_ERROR("Shader::unable to create vertex shader");
		std::string shader;
		readFile(vertexSRC, shader);
		shaderSource(_vertex, &shader[0]);
		glCompileShader(_vertex);
		checkCompilation(_vertex, vertexSRC);
		glAttachShader(_program, _vertex);
	}

	checkError();

	if (fragmentSRC.size())
	{
		_fragment = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragment == INVALID_PROGRAM_ID)
			VC_ERROR("Shader::unable to create fragment shader");
		std::string shader;
		readFile(fragmentSRC, shader);
		shaderSource(_fragment, &shader[0]);
		glCompileShader(_fragment);
		checkCompilation(_fragment, fragmentSRC);
		glAttachShader(_program, _fragment);
	}

	checkError();

	if (geometrySRC.size())
	{
		_geometry = glCreateShader(GL_GEOMETRY_SHADER);
		if (_geometry == INVALID_PROGRAM_ID)
			VC_ERROR("Shader::unable to create geometry shader");
		std::string shader;
		readFile(geometrySRC, shader);
		shaderSource(_geometry, &shader[0]);
		glCompileShader(_geometry);
		checkCompilation(_geometry, geometrySRC);
		glAttachShader(_program, _geometry);
	}

	checkError();
}

/////////////////////////////////////////////////////////////////////
////	Link program and check error
/////////////////////////////////////////////////////////////////////

void Shader::finalize(const char *name)
{
	glLinkProgram(_program);

	GLint isLinked = 0;
	glGetProgramiv(_program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_program, maxLength, &maxLength, &errorLog[0]);
		for (unsigned int i = 0; i < errorLog.size(); ++i)
			std::cout << errorLog[i];
		if (_program != 0)
			glDeleteProgram(_program);
		std::string err("Shader::unable to compile shader : " + std::string(name));
		VC_ERROR(err.c_str());
	}

	_name = name;

	checkError();
}

/////////////////////////////////////////////////////////////////////
////	Set varyings
/////////////////////////////////////////////////////////////////////

void Shader::setVaryings(const GLchar** varyings, const int &number)
{
	glTransformFeedbackVaryings(_program, number, varyings, GL_INTERLEAVED_ATTRIBS);
}

/////////////////////////////////////////////////////////////////////
////	INT VALUE **
/////////////////////////////////////////////////////////////////////

void Shader::setParameter(const GLchar* name, const int &value)
{
	glUniform1i(prepareParameter(name), value);
}

/////////////////////////////////////////////////////////////////////
////	FLOAT VALUE **
/////////////////////////////////////////////////////////////////////

void Shader::setParameter(const GLchar *name, const float &value)
{
	glUniform1f(prepareParameter(name), value);
}

/////////////////////////////////////////////////////////////////////
////	 VEC2 FLOAT VALUE **
/////////////////////////////////////////////////////////////////////

void Shader::setParameter(const GLchar *name, const glm::vec2 &value)
{
	glUniform2f(prepareParameter(name), value.x, value.y);
}

void Shader::setParameter(const GLchar *name, float value1, float value2)
{
	glUniform2f(prepareParameter(name), value1, value2);
}

/////////////////////////////////////////////////////////////////////
////	VEC3 FLOAT VALUE **
/////////////////////////////////////////////////////////////////////

void Shader::setParameter(const GLchar *name, const glm::vec3 &value)
{
	glUniform3f(prepareParameter(name), value.x, value.y, value.z);
}

void Shader::setParameter(const GLchar *name, const float &value, const float &value2, const float &value3)
{
	glUniform3f(prepareParameter(name), value, value2, value3);
}

/////////////////////////////////////////////////////////////////////
////	VEC4 FLOAT VALUE **
/////////////////////////////////////////////////////////////////////

void Shader::setParameter(const GLchar *name, const glm::vec4 &value)
{
	glUniform4f(prepareParameter(name), value.x, value.y, value.z, value.w);
}

void Shader::setParameter(const GLchar *name, const float &value, const float &value2, const float &value3, const float &value4)
{
	glUniform4f(prepareParameter(name), value, value2, value3, value4);
}

/////////////////////////////////////////////////////////////////////
////	MATRIX VALUE **
/////////////////////////////////////////////////////////////////////

void Shader::setParameter(const GLchar *name, const glm::mat4 &matrix)
{
	glUniformMatrix4fv(prepareParameter(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

// SFML version
void Shader::setParameter(const GLchar *name, const float *matrix)
{
	glUniformMatrix4fv(prepareParameter(name), 1, GL_FALSE, matrix);
}

/////////////////////////////////////////////////////////////////////
////	Bind/Unbind shader **
/////////////////////////////////////////////////////////////////////

void Shader::bind(void)
{
	// If the shader is already bind, ingore this call
	if (_program == currentProgram)
		return;

	glUseProgram(_program);
	currentProgram = _program;

	checkError();
}

void Shader::unbind(void)
{
	glUseProgram(0);
	currentProgram = 0;

	checkError();
}

/////////////////////////////////////////////////////////////////////
////	Get uniform location
/////////////////////////////////////////////////////////////////////

GLint Shader::prepareParameter(const GLchar *name)
{
	// If the shader is already bind, ingore this call
	if (_program != currentProgram)
		glUseProgram(_program);

	GLint location = glGetUniformLocation(_program, name);
	if (location == -1)
	{
		std::string warn("Shader::" + _name + ", uniform " + std::string(name) + " not found in shader");
		VC_WARNING(warn.c_str());
	}

	checkError();
	return location;
}

/////////////////////////////////////////////////////////////////////
////	Set source for shader
/////////////////////////////////////////////////////////////////////

void Shader::shaderSource(GLuint &shader, const std::string &src)
{
	const char *tmp = src.c_str();
	glShaderSource(shader, 1, &tmp, NULL);

	checkError();
}

/////////////////////////////////////////////////////////////////////
////	Read a file into a buffer
/////////////////////////////////////////////////////////////////////

void Shader::readFile(const std::string &filename, std::string& buffer)
{
	std::ifstream file(filename, std::ios_base::binary);
	if (file.good())
	{
		std::stringstream ss("");
		ss << file.rdbuf();
		buffer = ss.str();
		buffer.push_back('\0');
	}
	else
	{
		std::string err("Shader::file \"" + std::string(filename) + "\" doesn't exist");
		VC_ERROR(err.c_str());
	}
}

/////////////////////////////////////////////////////////////////////
////	Get compilation error
/////////////////////////////////////////////////////////////////////

void Shader::checkCompilation(const GLuint &shader, const std::string &name)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	std::stringstream ss("");
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	if (!maxLength)
		return;

	std::vector<char> errorLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
	for (unsigned int i = 0; i < errorLog.size(); ++i)
		ss << errorLog[i];
	if (isCompiled == GL_FALSE && shader != 0)
		glDeleteShader(shader);
	std::string err("Shader: " + std::string(name));
	err += "\n\n" + ss.str();

	if (isCompiled == GL_FALSE)
		VC_ERROR(err.c_str());
	else if (errorLog.size() > 5)
		VC_ERROR(err.c_str());

	checkError();
}

/////////////////////////////////////////////////////////////////////
////	Getters
/////////////////////////////////////////////////////////////////////

const GLuint& Shader::getProgramID(void)
{
	return _program;
}
