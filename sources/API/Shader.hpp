/********************************************************************
**
**              Shader.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef	OGL_SHADER
#define	OGL_SHADER

#include <vector>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "VoidClash_OpenGL.h"

#define	INVALID_PROGRAM_ID 0

namespace ogl
{
	class Shader
	{
	public:
		Shader(void);
		~Shader(void);
		void destroy(void);

		void loadFromFile(const std::string &vertexSRC, const std::string &fragmentSRC = std::string(""), const std::string &geometrySRC = std::string(""));
		void setVaryings(const GLchar **varyings, const int &number);
		void finalize(const char *name);

		void setParameter(const GLchar *name, const int &value);
		void setParameter(const GLchar *name, const float &value);
		void setParameter(const GLchar *name, const glm::vec2 &value);
		void setParameter(const GLchar *name, float value1, float value2);
		void setParameter(const GLchar *name, const glm::vec3 &value);
		void setParameter(const GLchar *name, const float &value, const float &value2, const float &value3);
		void setParameter(const GLchar *name, const glm::vec4 &value);
		void setParameter(const GLchar *name, const float &value, const float &value2, const float &value3, const float &value4);
		void setParameter(const GLchar *name, const glm::mat4 &matrix);
		void setParameter(const GLchar *name, const float *matrix);
		void setParameter(const GLchar *name, float *data, const unsigned int &size);
		void setParameter(const GLchar *name, float *data, const unsigned int &size, const unsigned int &cases);

		void bind(void);
		void unbind(void);

		const GLuint& getProgramID(void);

	private:
		void checkCompilation(const GLuint &shader, const std::string &name);
		void readFile(const std::string &filename, std::string& buffer);
		void shaderSource(GLuint &shader, const std::string& src);
		GLint prepareParameter(const GLchar *name);

	private:
		std::string _name;
		GLuint	_program;
		GLuint	_vertex;
		GLuint	_fragment;
		GLuint	_geometry;
	};
}

#endif
