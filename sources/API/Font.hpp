/********************************************************************
**
**              Font.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef __OGL_FONT__
#define __OGL_FONT__

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "VoidClash_OpenGL.h"

namespace ogl
{
	struct glyphAttrib
	{
		glyphAttrib(void)
		{
			ax = 0; ay = 0; bw = 0; bh = 0;
			bl = 0; bt = 0; tx = 0; ty = 0;
		}

		float ax; // advance.x
		float ay; // advance.y

		float bw; // bitmap.width;
		float bh; // bitmap.rows;

		float bl; // bitmap_left;
		float bt; // bitmap_top;

		float tx; // x offset of glyph in texture coordinates
		float ty; // y offset of glyph in texture coordinates
	};

	class Font
	{
	public:
		Font(void);
		~Font(void);

		// Load a font
		void loadFromFile(const std::string &font, int size);

		// Delete the font
		void destroy(void);

		// Getters
		GLuint getTextureID(void) { return _texID; }
		std::vector<glyphAttrib>& getGlyph(void) { return _glyph; }
		glm::ivec2 &getAtlasSize(void) { return _atlasSize; }
		int getOffsetY(void) { return _maxHeight; }

		// Debug
		void debug(void);

	private:
		void calculateAltasSize(void);
		void createTexture(void);

	private:
		// Care about double loading
		bool _loaded;

		// Face
		FT_Face _face;

		// Atlas size
		glm::ivec2 _atlasSize;
		int _maxHeight;

		// Glyph attributes
		std::vector<glyphAttrib> _glyph;

		// OpenGL stuff
		GLuint _texID;
	};
}

#endif
