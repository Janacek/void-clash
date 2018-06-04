/********************************************************************
**
**              Font.cpp
**              Created by : Vial Joris
**				http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02
**
********************************************************************/

#include <algorithm>
#include "Font.hpp"
#include "Text.hpp"
#include "CheckError.hpp"
#include "Log.hpp"

using namespace ogl;

#define MAXWIDTH 1024

/////////////////////////////////////////////////////////////////////
/////	Constructor / Destructor
/////////////////////////////////////////////////////////////////////

Font::Font(void)
{
	_loaded = false;
}

Font::~Font(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Load a font
/////////////////////////////////////////////////////////////////////

void Font::loadFromFile(const std::string &font, int size)
{
	// Check if we don't already have load this font
	if (_loaded == true)
		return;

	VC_INFO("Font::Loading " + font + ", size " + std::to_string(size));

	// Prepare glyph container
	_glyph.resize(256);

	std::string fullPath(font);
	if (FT_New_Face(Text::getFreetypeLibrary(), fullPath.c_str(), 0, &_face) != 0)
		VC_ERROR("Font::Unable to load font " + font);

	// Set pixel size
	FT_Set_Pixel_Sizes(_face, 0, size);

	// Calculate the altas size
	calculateAltasSize();

	// Create texture
	createTexture();

	_loaded = true;

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Calculate atlas size
/////////////////////////////////////////////////////////////////////

void Font::calculateAltasSize(void)
{
	int roww = 0;
	int rowh = 0;
	_maxHeight = 0;

	_atlasSize.x = 0;
	_atlasSize.y = 0;

	FT_GlyphSlot g = _face->glyph;

	for (int i = 0; i < 256; ++i)
	{
		if (FT_Load_Char(_face, i, FT_LOAD_RENDER) != 0)
		{
			VC_WARNING("Font::Loading character " +
				std::to_string((char)i) + " failed!");
		}

		roww += g->bitmap.width + 1;
		rowh = std::max(rowh, g->bitmap.rows);
		_maxHeight = std::max(_maxHeight, g->bitmap_top);
	}

	_atlasSize.x = std::max(_atlasSize.y, roww);
	_atlasSize.y += rowh;
}

/////////////////////////////////////////////////////////////////////
/////	Create texture
/////////////////////////////////////////////////////////////////////

void Font::createTexture(void)
{
	// Generate texture
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _atlasSize.x,
		_atlasSize.y, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Paste glyph bitmap into the texture
	int ox = 0;
	int oy = 0;
	int rowh = 0;

	FT_GlyphSlot g = _face->glyph;

	for (int i = 0; i < 256; ++i)
	{
		if (FT_Load_Char(_face, i, FT_LOAD_RENDER) != 0)
		{
			VC_WARNING("Font::Loading character " +
				std::to_string((char)i) + " failed!");
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width,
			g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE,
			g->bitmap.buffer);

		_glyph[i].ax = g->advance.x >> 6;
		_glyph[i].ay = g->advance.y >> 6;
		_glyph[i].bw = g->bitmap.width;
		_glyph[i].bh = g->bitmap.rows;
		_glyph[i].bl = g->bitmap_left;
		_glyph[i].bt = g->bitmap_top;
		_glyph[i].tx = ox / (float)_atlasSize.x;
		_glyph[i].ty = oy / (float)_atlasSize.y;

		rowh = std::max(rowh, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Delete the font
/////////////////////////////////////////////////////////////////////

void Font::destroy(void)
{
	_loaded = false;
	_glyph.clear();
	glDeleteTextures(1, &_texID);

	checkError();
}
