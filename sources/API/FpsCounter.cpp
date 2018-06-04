/********************************************************************
**
**              FpsCounter.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "FpsCounter.hpp"
#include "Context.hpp"
#include "AssetPath.h"
#include "Log.hpp"

using namespace ogl;

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

FpsCounter::FpsCounter(void)
{
}

FpsCounter::~FpsCounter(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Fps counter initialization
/////////////////////////////////////////////////////////////////////

void FpsCounter::start(void)
{
	_totalTime = 0;
	_framePerSeconds = 0;

	_drawable = false;

	_font = new ogl::Font();
	_font->loadFromFile(ASSETS_PATH + "assets/fonts/ExoBlack.otf", 36);

	_text = new ogl::Text();
	_text->create(_font, "");
}

/////////////////////////////////////////////////////////////////////
/////	Fps counter destroy
/////////////////////////////////////////////////////////////////////

void FpsCounter::destroy()
{
	_text->destroy();
	delete _text;

	_font->destroy();
	delete _font;

	_drawable = false;
	_framePerSeconds = 0;
	_totalTime = 0;
}

/////////////////////////////////////////////////////////////////////
/////	Fps counter update
/////////////////////////////////////////////////////////////////////

void FpsCounter::update(float deltaTime)
{
	++_framePerSeconds;
	_totalTime += deltaTime;

	display();
}

/////////////////////////////////////////////////////////////////////
/////	Fps counter display
/////////////////////////////////////////////////////////////////////

void FpsCounter::display(void)
{
	if (_totalTime > 1.0f)
	{
		// VC_INFO("Fps: " + std::to_string(_framePerSeconds));

		// To debug font
		// _text->setString("abcdefghijklmnopqrstuvwxyz0123456789²&é\"'(-è_çà)=$*ù:;,1234567890°+£µ%/.");

		// Update string
		_text->setString("FPS: " + std::to_string(_framePerSeconds));

		// Update color
		_text->setColor(glm::vec4(0, 1, 0, 1));
		if (_framePerSeconds < 30)
			_text->setColor(glm::vec4(1, 0, 0, 1));
		else if (_framePerSeconds < 60)
			_text->setColor(glm::vec4(1, 1, 0, 1));

		// Set position
		_text->resetTransform();
		_text->setScale(0.5f, -0.5f);
		_text->setPosition(0.0f, _text->getOffsetY());

		_framePerSeconds = 0;
		_totalTime = 0;

		_drawable = true;
	}

	if (_drawable == false)
		return;

	_text->draw();
}
