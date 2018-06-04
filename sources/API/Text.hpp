/********************************************************************
**
**              Text.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_TEXT
#define OGL_TEXT

#include "Shader.hpp"
#include "Font.hpp"
#include "Transformable.hpp"
#include "Camera.hpp"

namespace ogl
{
	// Describe glyph vertices
	struct glyphVertex
	{
		glyphVertex(void)
		{
			x = 0; y = 0; s = 0; t = 0;
		}
		glyphVertex(float x1, float y1, float s1, float t1)
		{
			x = x1; y = y1; s = s1; t = t1;
		}
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	class Text : public Transformable
	{
	public:
		Text(void);
		~Text(void);

		void create(Font *font, const std::string &s, Camera *camera = NULL);
		void draw(void);
		void destroy(void);

		// Setters
		void setString(const std::string &s);
		void setColor(const glm::vec4 &color);
		void setScale(float sx, float sy);
		void setPosition(float px, float py);
		void setRotation(float angle);
		void setFont(ogl::Font *font);

		// Getters
		glm::vec2 getSize(void);
		int getOffsetY(void);

		// Freetype library management
		static void initializeFreetype(void);
		static void releaseFreetype(void);
		static FT_Library& getFreetypeLibrary(void);

	private:
		void generateVertices(void);
		void updateBuffer(void);

	private:
		// Rendering
		Camera *_camera;
		Font *_font;

	public:
		// Property
		glm::vec2 _size;
		glm::vec2 _scale;
		glm::vec4 _color;
		std::string _currentString;

		// Other
		bool _deleteCamera;

		// OpenGL
		GLuint _vao;
		GLuint _vbo;
		GLuint _vertexNbr;

		// Temporary
		glyphVertex *_coords;
	};
}

#endif
