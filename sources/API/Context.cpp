/********************************************************************
**
**              Context.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "Context.hpp"
#include "Log.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Instance
/////////////////////////////////////////////////////////////////////

Context *Context::_instance = NULL;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Context::Context(void)
{
	_windowCreated = false;
}

Context::~Context(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Free ressources
/////////////////////////////////////////////////////////////////////

void Context::destroyRessources(void)
{
	// Release freetype library
	ogl::Text::releaseFreetype();

	// Close window and GLFW
	glfwDestroyWindow(_window);
	glfwTerminate();
	_windowCreated = false;

	Context *tmp = _instance;
	_instance = NULL;
	delete tmp;
}

/////////////////////////////////////////////////////////////////////
/////	Verifiy if we have an instance and return it
/////////////////////////////////////////////////////////////////////

Context* Context::self(void)
{
	if (_instance == NULL)
		_instance = new Context;
	return _instance;
}

/////////////////////////////////////////////////////////////////////
/////	Window error callback
/////////////////////////////////////////////////////////////////////

static void error_callback(int error, const char *description)
{
	VC_ERROR("Context::GLFWwindow error " + std::to_string(error) +
		+ ", " + std::string(description));
}

/////////////////////////////////////////////////////////////////////
/////	Window creation
/////////////////////////////////////////////////////////////////////

void Context::initGLFW(void)
{
	// Set error callback
	glfwSetErrorCallback(error_callback);

	// Init library
	if (!glfwInit())
		VC_ERROR("Context::Unable to init GLFW");
}

void Context::createWindow(const GLFWvidmode &mode, const std::string &title,
	bool fullscreen)
{
	// Get current video mode
	glfwWindowHint(GLFW_RED_BITS, mode.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode.blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode.refreshRate);

	// Request a OpenGL 3.3 Core Context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	if (fullscreen == false)
		_window = glfwCreateWindow(mode.width, mode.height,
			title.c_str(), NULL, NULL);
	else
		_window = glfwCreateWindow(mode.width, mode.height,
		title.c_str(), glfwGetPrimaryMonitor(), NULL);

	if (_window == NULL)
	{
		glfwTerminate();
		VC_ERROR("Context::Failed to create window");
	}

	// Set OpenGL context as current
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	// Since we want a OpenGL 3.3 Core Context, we need to enable
	// glueExperimental to access every extensions
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
		VC_ERROR_CRITICAL("Context::Unable to initialize glew", true);
	if (!GLEW_VERSION_3_3)
		VC_ERROR_CRITICAL("Context::You need at least OpenGL 3.3, check your graphic card opengl support and update your drivers", true);

	// Ignore fake OpenGL error caused by glew initialization
	while (glGetError() != GL_NO_ERROR);

	// Enable mutisample
	glEnable(GL_MULTISAMPLE);

	_windowCreated = true;

	VC_INFO("Context::OpenGL context successfully created");
}

/////////////////////////////////////////////////////////////////////
/////	 Initialize freetype
/////////////////////////////////////////////////////////////////////

void Context::initializeFreetype(void)
{
	ogl::Text::initializeFreetype();
}

/////////////////////////////////////////////////////////////////////
/////	Display the SFML window
/////////////////////////////////////////////////////////////////////

void Context::update(void)
{
	if (!_windowCreated)
	{
		VC_WARNING("Context::Trying to use window, but it wasn't created yet");
		return;
	}
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

/////////////////////////////////////////////////////////////////////
/////	Getters
/////////////////////////////////////////////////////////////////////

GLFWwindow& Context::getWindow(void)
{
	if (_windowCreated == false)
		VC_WARNING("trying to get window, but it wasn't created yet");
	return *_window;
}

glm::ivec2 Context::getWindowSize(void)
{
	glm::ivec2 s;
	glfwGetWindowSize(_window, &(s.x), &(s.y));
	return s;
}

int Context::windowIsOpen(void)
{
	return !glfwWindowShouldClose(_window);
}

char Context::oglDebugLevel(void)
{
	return DEBUG_LEVEL;
}
