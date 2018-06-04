/********************************************************************
**
**              GraphicHudWarmup.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicHudWarmup.hpp"
#include "GraphicConfiguration.hpp"
#include "Map.hpp"
#include "GraphicHelper.hpp"

namespace VoidClashGraphics
{
	// Message attributes
	#define WARMUP_MESSAGE_POSX (GCONFIG.winX * 0.5f)
	#define WARMUP_MESSAGE_POSY 120.0f

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicHudWarmup::GraphicHudWarmup(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicHudWarmup::~GraphicHudWarmup(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicHudWarmup::start(ogl::Camera *camera)
	{
		std::string font("ExoRegular.otf");

		// Create text
		_text = new ogl::Text();
		_text->create(S_Font->getFont(font), "--- Echauffement ---", camera);

		// Setup text
		_text->resetTransform();
		_text->setScale(1.0f, -1.0f);
		_text->setPosition(
			WARMUP_MESSAGE_POSX - _text->getSize().x * 0.5f,
			WARMUP_MESSAGE_POSY);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicHudWarmup::update(float deltatime)
	{
		(void)deltatime;

		// Display message only during warmup
		if (S_Map->getWarmupTime().asSeconds() <= 1)
			return;

		// Draw the text
		_text->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicHudWarmup::stop(void)
	{
		_text->destroy();
		delete _text;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL states
	/////////////////////////////////////////////////////////////////////

	void GraphicHudWarmup::clearOpenGLStates(void)
	{

	}
}
