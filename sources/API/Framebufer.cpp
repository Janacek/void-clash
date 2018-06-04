/********************************************************************
**
**              Framebuffer.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <cstring>
#include "Framebuffer.hpp"
#include "CheckError.hpp"
#include "Log.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Constructor / Destructor
/////////////////////////////////////////////////////////////////////

Framebuffer::Framebuffer(void)
{
	_texture = NULL;
	_fbo = 0;
	_depth = 0;
	_textNbr = 9999999;
}

Framebuffer::~Framebuffer(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Creation
/////////////////////////////////////////////////////////////////////

void Framebuffer::create(int sizeX, int sizeY, bool depthBuffer, int repeatMode,
	unsigned int textNbr, unsigned int sampleNumber)
{
	_size = glm::vec2(sizeX, sizeY);
	_textNbr = textNbr;
	_depthBuffer = depthBuffer;
	_repeat = repeatMode;
	_sampleNumber = sampleNumber;

	_texture = new GLuint[textNbr];
	std::memset(_texture, 0, sizeof(GLuint) * textNbr);

	// Create OpenGL framebuffer
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	checkError();

	// Create texture
	glGenTextures(textNbr, _texture);

	for (unsigned int i = 0; i < textNbr; ++i)
	{
		if (sampleNumber == 0)
		{
			glBindTexture(GL_TEXTURE_2D, _texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _repeat);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _repeat);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, getColorAttachment(i), GL_TEXTURE_2D, _texture[i], 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _repeat);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _repeat);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sampleNumber, GL_RGBA8, _size.x, _size.y, GL_TRUE);
			checkError();
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			checkError();
			glFramebufferTexture2D(GL_FRAMEBUFFER, getColorAttachment(i), GL_TEXTURE_2D_MULTISAMPLE, _texture[i], 0);
		}

		checkError();
	}

	// Generate depth buffer
	if (_depthBuffer == true)
	{
		glGenRenderbuffers(1, &_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, _depth);

		if (sampleNumber == 0)
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _size.x, _size.y);
		else
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleNumber, GL_DEPTH_COMPONENT24, _size.x, _size.y);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);

		checkError();
	}

	// Check completion
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		return printFramebufferError(status);

	checkError();

	// Clean
	unbind();
}

void Framebuffer::destroy(void)
{
	if (_texture == 0)
		return;

	glDeleteTextures(_textNbr, _texture);
	glDeleteFramebuffers(1, &_fbo);
	glDeleteRenderbuffers(1, &_depth);

	checkError();

	delete[] _texture;

	_texture = NULL;
	_fbo = 0;
	_depth = 0;
}

/////////////////////////////////////////////////////////////////////
/////	GetColorAttachment
/////////////////////////////////////////////////////////////////////

GLuint Framebuffer::getColorAttachment(int i)
{
	switch (i)
	{
	case 0: return GL_COLOR_ATTACHMENT0;
	case 1: return GL_COLOR_ATTACHMENT1;
	case 2: return GL_COLOR_ATTACHMENT2;
	case 3: return GL_COLOR_ATTACHMENT3;
	case 4: return GL_COLOR_ATTACHMENT4;
	default: return GL_COLOR_ATTACHMENT0;
	}
}


/////////////////////////////////////////////////////////////////////
/////	Bind / Unbind framebuffer
/////////////////////////////////////////////////////////////////////

void Framebuffer::bind(void)
{
	if (_textNbr == 9999999)
		return;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	GLuint *attachment = new GLuint[_textNbr];
	for (unsigned int i = 0; i < _textNbr; ++i)
		attachment[i] = getColorAttachment(i);
	glDrawBuffers(_textNbr, attachment);
	delete[] attachment;

	checkError();
}

void Framebuffer::unbind(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Bind / Unbind Texture
/////////////////////////////////////////////////////////////////////

void Framebuffer::bindTexture(char unit, unsigned int textNbr)
{
	if (unit == 0)
		glActiveTexture(GL_TEXTURE0);
	else if (unit == 1)
		glActiveTexture(GL_TEXTURE1);
	else if (unit == 2)
		glActiveTexture(GL_TEXTURE2);
	else if (unit == 3)
		glActiveTexture(GL_TEXTURE3);
	else if (unit == 4)
		glActiveTexture(GL_TEXTURE4);
	else if (unit == 5)
		glActiveTexture(GL_TEXTURE5);

	if (_sampleNumber > 1)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture[textNbr]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, _texture[textNbr]);
	}

	checkError();
}

void Framebuffer::unbindTexture(void)
{
	if (_sampleNumber > 1)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Print
/////////////////////////////////////////////////////////////////////

void Framebuffer::printFramebufferError(GLenum error)
{
	switch (error)
	{
		case GL_FRAMEBUFFER_UNDEFINED:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_UNDEFINED"); break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_UNSUPPORTED"); break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); break;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); break;

		default:
		VC_ERROR_CRITICAL("Framebuffer::GL_FRAMEBUFFER_UNDEFINED"); break;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Get size
/////////////////////////////////////////////////////////////////////

const glm::vec2& Framebuffer::getSize(void)
{
	return _size;
}

GLuint Framebuffer::getId(void)
{
	return _fbo;
}

/////////////////////////////////////////////////////////////////////
/////	Resize the framebuffer
/////////////////////////////////////////////////////////////////////

void Framebuffer::resize(int sizeX, int sizeY)
{
	destroy();
	create(sizeX, sizeY, _depthBuffer, _repeat, _textNbr);
}
