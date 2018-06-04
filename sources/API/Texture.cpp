/********************************************************************
**
**              Texture.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Texture.hpp"
#include "CheckError.hpp"
#include "SOIL2.h"
#include "Log.hpp"

using namespace ogl;

static GLuint currentTexture = 0;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Texture::Texture(void)
{
	_id = 0;
	_image = NULL;
}

Texture::~Texture(void)
{
	// Basic loading (no OpenGL)
	if (_image != NULL)
	{
		SOIL_free_image_data(_image);
		_image = NULL;
		return;
	}

	// OpenGL
	if (_id == 0)
		return;

	glDeleteTextures(1, &_id);
	_id = 0;
	currentTexture = 0;

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Load texture from file
/////////////////////////////////////////////////////////////////////

bool Texture::create(const std::string &filename, int repeatMode)
{
	VC_INFO("Texture::Loading " + filename);

	// Check if we have already created texture
	if (_id != 0)
	{
		std::string warn("texture id " +
			std::to_string(_id) +
			std::string(" already created"));
		VC_WARNING(warn.c_str());
		return true;
	}

	// Load the texture
	std::string fullPath(filename);
	_id = SOIL_load_OGL_texture(
				    fullPath.c_str(),
					SOIL_LOAD_AUTO,
				    SOIL_CREATE_NEW_ID,
				    SOIL_FLAG_INVERT_Y
				    );

	// Loading failed ?
	if (_id <= 0)
		VC_ERROR("Texture::unable to load texture : \"" + std::string(filename) + "\"");

	// Set texture parameters
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);
	glBindTexture(GL_TEXTURE_2D, 0);

	checkError();

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Load a texture without creating OpenGL stuff
/////////////////////////////////////////////////////////////////////

unsigned char* Texture::loadFromFile(const char *filename, int &width, int &height)
{
	std::string fullPath = filename;

	_image = SOIL_load_image(fullPath.c_str(), &width, &height, 0, SOIL_LOAD_AUTO);

	return _image;
}

/////////////////////////////////////////////////////////////////////
/////	Bind/Unbind texture
/////////////////////////////////////////////////////////////////////

void Texture::bind(char unit)
{
	// Texture::already bind, ignore this call
	if (_id == currentTexture)
		return;

	// Bind the texture on the good texture unit
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

	glBindTexture(GL_TEXTURE_2D, _id);
	currentTexture = _id;

	checkError();
}

void Texture::unbind(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	currentTexture = 0;

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Getters
/////////////////////////////////////////////////////////////////////

GLuint Texture::getId(void)
{
	return _id;
}
