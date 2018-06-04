/********************************************************************
**
**              Texture.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_TEXTURE
#define OGL_TEXTURE

#include <string>
#include "VoidClash_OpenGL.h"

namespace ogl
{
	class Texture
	{
	public:
		Texture(void);
		~Texture(void);

		bool create(const std::string &filename, int repeatParam = GL_REPEAT);
		void bind(char unit = 0);
		void unbind(void);

		// Load a texture without creating OpenGL stuff
		unsigned char* loadFromFile(const char *filename, int &width, int &height);

		GLuint getId(void);

	private:
		GLuint _id;

		// No OpenGL stuff
		unsigned char *_image;
	};
}

#endif
