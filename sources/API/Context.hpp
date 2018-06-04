/********************************************************************
**
**              Context.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_CONTEXT
#define OGL_CONTEXT

#include <string>
#include "Text.hpp"
#include "VoidClash_OpenGL.h"

namespace ogl
{
	class Context
	{
	public:
		Context(void);
		~Context(void);

		static Context* self(void);

		void initGLFW(void);
		void createWindow(const GLFWvidmode &mode, const std::string &title,
			bool fullcreen);
		void destroyRessources(void);
		void initializeFreetype(void);
		void update(void);

		// Get window attributes
		GLFWwindow& getWindow(void);
		glm::ivec2 getWindowSize(void);
		int windowIsOpen(void);

		// Get debug level
		char oglDebugLevel(void);

		// Instance
		static Context *_instance;

	private:
		// Current state
		bool _windowCreated;
		GLFWwindow *_window;
		GLint _initMem;
	};
}

#define S_Context ogl::Context::self()

#endif
