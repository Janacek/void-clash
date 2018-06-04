/********************************************************************
**
**              Text.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <algorithm>
#include "Text.hpp"
#include "CheckError.hpp"
#include "Log.hpp"
#include "Context.hpp"

using namespace ogl;

// Freetype library handler
static FT_Library ft;

// Rendering shader
static Shader *shader;

/////////////////////////////////////////////////////////////////////
/////	Ctor / Dtor
/////////////////////////////////////////////////////////////////////

Text::Text(void)
{
	_deleteCamera = false;
	_coords = NULL;
	_vao = 0;
	_vbo = 0;
}

Text::~Text(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Freetype library management and common datas
/////////////////////////////////////////////////////////////////////

void Text::initializeFreetype(void)
{
	VC_INFO("Text::Initializing freetype library");

	// Load rendering shader
	shader = new ogl::Shader();
	shader->loadFromFile("../assets/shaders/Text.vert", "../assets/shaders/Text.frag");
	shader->finalize("Text rendering");

	// Initialize freetype
	if (FT_Init_FreeType(&ft) != 0)
		VC_ERROR("Text::Cannot initialize freetype library");

	checkError();
}

void Text::releaseFreetype(void)
{
	// Release freetype
	FT_Done_FreeType(ft);

	checkError();

	// Delete shader
	shader->destroy();
	delete shader;

	checkError();
}

FT_Library& Text::getFreetypeLibrary(void)
{
	return ft;
}

/////////////////////////////////////////////////////////////////////
/////	Initialize all modules
/////////////////////////////////////////////////////////////////////

void Text::create(Font *font, const std::string &s, Camera *camera)
{
	_color = glm::vec4(1, 1, 1, 1);
	_scale.x = 1.0; _scale.y = 1.0;

	// Set text property
	_camera = camera;
	_font = font;
	_currentString = s;

	// Create VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create VBO
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Generate vertices
	generateVertices();

	// Send it to the GPU
	updateBuffer();

	// Done
	glBindVertexArray(0);

	// If not camera are setted, create one (SFML like)
	if (_camera == NULL)
	{
		_camera = new Camera();
		_camera->setupOrthographic(S_Context->getWindowSize().x, S_Context->getWindowSize().y); // TOCHANGE
		_camera->update();
		_deleteCamera = true;
	}

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Draw
/////////////////////////////////////////////////////////////////////

void Text::draw(void)
{
	// No font setted, ignore call
	if (_font == NULL)
	{
		VC_WARNING("Text::Unable to render text, you must set a font before (Text::setFont(Font*))");
		return;
	}

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, _font->getTextureID());

	// Bind shader
	shader->bind();
	shader->setParameter("Model", getTransform());
	shader->setParameter("Color", _color);
	shader->setParameter("Camera", _camera->getFinal());

	// Draw
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _vertexNbr);
	glBindVertexArray(0);

	// Clean
	glBindTexture(GL_TEXTURE_2D, 0);
	shader->unbind();

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Generate the VBO
/////////////////////////////////////////////////////////////////////

void Text::generateVertices(void)
{
	if (_font == NULL)
	{
		VC_WARNING("Text::Unable to render text, you must set a font before (Text::setFont(Font*))");
		return;
	}

	int c = 0;
	int x = 0;
	int y = 0;
	_coords = new glyphVertex[6 * _currentString.length()];
	std::vector<glyphAttrib> &g(_font->getGlyph());
	glm::ivec2 &s(_font->getAtlasSize());

	for (const unsigned char *p = (unsigned char *)_currentString.c_str(); *p; p++)
	{
		/* Calculate the vertex and texture coordinates */
		float x2 = g[*p].bl;
		float y2 = -g[*p].bt;
		float w = g[*p].bw;
		float h = g[*p].bh;

		/* Skip glyphs that have no pixels */
		if ((!w || !h) && *p != ' ')
			continue;

		_coords[c++] = glyphVertex(
			x + x2, -y2, g[*p].tx, g[*p].ty
			);
		_coords[c++] = glyphVertex(
			x + x2 + w, -y2, g[*p].tx + g[*p].bw / s.x, g[*p].ty
			);
		_coords[c++] = glyphVertex(
			x + x2, -y2 - h, g[*p].tx, g[*p].ty + g[*p].bh / s.y
			);
		_coords[c++] = glyphVertex(
			x + x2 + w, -y2, g[*p].tx + g[*p].bw / s.x, g[*p].ty
			);
		_coords[c++] = glyphVertex(
			x + x2, -y2 - h, g[*p].tx, g[*p].ty + g[*p].bh / s.y
			);
		_coords[c++] = glyphVertex(
			x + x2 + w, -y2 - h, g[*p].tx + g[*p].bw / s.x, g[*p].ty + g[*p].bh / s.y
			);

		/* Advance the cursor to the start of the next character */
		x += g[*p].ax;
		y += g[*p].ay;

		_size.y = std::max((float)_size.y, (float)g[*p].bh);
	}

	_size.x = x;
	_vertexNbr = c;
}

/////////////////////////////////////////////////////////////////////
/////	Send vertices on GPU
/////////////////////////////////////////////////////////////////////

void Text::updateBuffer(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glyphVertex) * 6 * _currentString.length(), _coords, GL_STREAM_DRAW);

	delete[] _coords;
	_coords = NULL;

	checkError();
}

/////////////////////////////////////////////////////////////////////
/////	Update string
/////////////////////////////////////////////////////////////////////

void Text::setString(const std::string &s)
{
	// Check if we need to re-compute the vertices
	if (_currentString != s)
	{
		_currentString = s;

		// Generate new vertices
		generateVertices();

		// Update GPU
		updateBuffer();
	}
}

/////////////////////////////////////////////////////////////////////
/////	Set color
/////////////////////////////////////////////////////////////////////

void Text::setColor(const glm::vec4 &color)
{
	_color = color;
}

/////////////////////////////////////////////////////////////////////
/////	Set scale
/////////////////////////////////////////////////////////////////////

void Text::setScale(float sx, float sy)
{
	_scale.x = sx;
	_scale.y = std::fabs(sy);
	scale(sx, sy);
}

/////////////////////////////////////////////////////////////////////
/////	Set position
/////////////////////////////////////////////////////////////////////

void Text::setPosition(float px, float py)
{
	translate(px, py);
}

/////////////////////////////////////////////////////////////////////
/////	Set scale
/////////////////////////////////////////////////////////////////////

void Text::setRotation(float angle)
{
	rotate(angle, ogl::Zaxis);
}

/////////////////////////////////////////////////////////////////////
/////	Change the font
/////////////////////////////////////////////////////////////////////

void Text::setFont(ogl::Font *font)
{
	if (font != _font)
	{
		_font = font;

		if (_currentString.size() != 0)
		{
			// Generate new vertices
			generateVertices();

			// Update GPU
			updateBuffer();
		}
	}
}

/////////////////////////////////////////////////////////////////////
/////	Get bounding size
/////////////////////////////////////////////////////////////////////

glm::vec2 Text::getSize(void)
{
	return glm::vec2(_size.x * _scale.x, _size.y * _scale.y);
}

/////////////////////////////////////////////////////////////////////
/////	Get offset Y (from origin line to top Y char)
/////////////////////////////////////////////////////////////////////

int Text::getOffsetY(void)
{
	return _font->getOffsetY() * _scale.y;
}

/////////////////////////////////////////////////////////////////////
/////	Destroy
/////////////////////////////////////////////////////////////////////

void Text::destroy(void)
{
	// Delete buffers
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);

	// Delete camera if needed
	if (_deleteCamera == true)
		delete _camera;

	checkError();
}
