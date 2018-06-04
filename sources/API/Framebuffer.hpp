/********************************************************************
**
**              Framebuffer.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_FRAMEBUFFER
#define OGL_FRAMEBUFFER

#include "VoidClash_OpenGL.h"
#include "glm/glm.hpp"

namespace ogl
{
	class Framebuffer
	{
	public:
		Framebuffer(void);
		~Framebuffer(void);

		void create(int sizeX, int sizeY, bool depthBuffer = true,
			int repeat = GL_CLAMP_TO_EDGE, unsigned int textNbr = 1,
			unsigned int sampleNumber = 0);
		void destroy(void);
		void resize(int sizeX, int sizeY);

		void bind(void);
		void unbind(void);

		void bindTexture(char unit = 0, unsigned int textNbr = 0);
		void unbindTexture(void);

		const glm::vec2& getSize(void);
		GLuint getId(void);

	private:
		void printFramebufferError(GLenum error);
		GLuint getColorAttachment(int i);

	private:
		GLuint *_texture;
		GLuint _fbo;
		GLuint _depth;

		glm::vec2 _size;
		bool _depthBuffer;
		int _repeat;
		unsigned int _textNbr;
		unsigned int _sampleNumber;
	};
}

#endif
