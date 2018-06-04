/********************************************************************
**
**              FontManager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "FontManager.hpp"
#include "Log.hpp"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	CFontManager::CFontManager(void)
	{
	}

	CFontManager::~CFontManager(void)
	{
	}

	/////////////////////////////////////////////////////////////////////
	/////	Initialize
	/////////////////////////////////////////////////////////////////////

	void CFontManager::start(void)
	{
		VC_INFO("FontManager::starting");

		//////// Load fonts for Void Clash ////////

		// Lekton
		addFont("LektonBold.ttf");
		addFont("LektonRegular.ttf");

		// Not courrier
		addFont("NotCourierSans.otf");
		addFont("NotCourierSansBold.otf");

		// Exo
		addFont("ExoRegular.otf");
		addFont("ExoSemiBold.otf");
		addFont("ExoBold.otf");
		addFont("ExoBlack.otf");

		// Jura
		addFont("JuraRegular.ttf");
		addFont("JuraMedium.ttf");

		// Zorque
		addFont("Zorque.ttf");

		// Gasalt
		addFont("GasaltBlack.ttf");

		// Unispace
		addFont("UnispaceRegular.ttf");

		VC_INFO("FontManager::started");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void CFontManager::stop(void)
	{
		VC_INFO("FontManager::stopping");

		auto it = _fonts.begin();
		auto end = _fonts.end();

		while (it != end)
		{
			it->second->destroy();
			delete(it->second);
			++it;
		}

		_fonts.clear();

		VC_INFO("FontManager::stopped");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Load font if it doesn't exists and keep it into map
	/////////////////////////////////////////////////////////////////////

	ogl::Font *CFontManager::addFont(const std::string &name,
		ogl::Font *fontIN)
	{
		auto it = _fonts.find(name);

		if (it == _fonts.end())
		{
			if (fontIN == NULL)
			{
				std::string file(_root + name);
				ogl::Font *font = new ogl::Font();
				font->loadFromFile(file.c_str(), 48);
				_fonts[name] = font;
				return font;
			}
			else
			{
				_fonts[name] = fontIN;
				return fontIN;
			}
		}
		return _fonts[name];
	}

	/////////////////////////////////////////////////////////////////////
	/////	Set the root path where assets will be loaded
	/////////////////////////////////////////////////////////////////////

	void CFontManager::setRootDirectory(const std::string &root)
	{
		_root = root;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Return a pointer on the font
	/////////////////////////////////////////////////////////////////////

	ogl::Font* CFontManager::getFont(const std::string &name)
	{
		auto it = _fonts.find(name);

		if (it == _fonts.end())
			return NULL;

		return it->second;
	}
}
