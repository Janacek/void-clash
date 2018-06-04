/********************************************************************
**
**              CheckError.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <stdexcept>
#include <sstream>
#include "VoidClash_OpenGL.h"
#include "CheckError.hpp"

#include <iostream>
#include "Log.hpp"

namespace ogl
{
	void _check_gl_error(const char *file, int line)
	{
		if (ENABLE_OPENGL_CHECK == false)
			return;

		bool needExit = false;
		GLenum error = GL_NO_ERROR;
		std::stringstream o("");
		std::string File(file);
		unsigned int slash = File.find_last_of("/\\");
		File = File.substr(slash + 1);

		while ((error = glGetError()) != GL_NO_ERROR)
		{
			switch (error) {
			case GL_NO_ERROR:
				break;

			case GL_INVALID_ENUM:
				o << "OpenGL Error in " << File << " at line " << line << ": Invalid enum!" << std::endl;
				needExit = true;
				break;

			case GL_INVALID_VALUE:
				o << "OpenGL Error in " << File << " at line " << line << ": Invalid value!" << std::endl;
				needExit = true;
				break;

			case GL_INVALID_OPERATION:
				o << "OpenGL Error in " << File << " at line " << line << ": Invalid operation!" << std::endl;
				needExit = true;
				break;

			case GL_STACK_OVERFLOW:
				o << "OpenGL Error in " << File << " at line " << line << ": Stack overflow!" << std::endl;
				needExit = true;
				break;

			case GL_STACK_UNDERFLOW:
				o << "OpenGL Error in " << File << " at line " << line << ": Stack underflow!" << std::endl;
				needExit = true;
				break;

			case GL_OUT_OF_MEMORY:
				o << "OpenGL Error in " << File << " at line " << line << ": Out Of memory!" << std::endl;
				needExit = true;
				break;

			case GL_TABLE_TOO_LARGE:
				o << "OpenGL Error in " << File << " at line " << line << ": Table too large!" << std::endl;
				needExit = true;
				break;

			default:
				o << "OpenGL Error in " << File << " at line " << line << ": Unknown error! N°" << error << std::endl;
				needExit = true;
				break;
			}
		}
		if (needExit == true)
			std::cout << o.str().c_str() << std::endl;
	}
}
