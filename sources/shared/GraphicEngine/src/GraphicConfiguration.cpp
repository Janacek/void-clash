/********************************************************************
**
**              GraphicConfiguration.hpp
**              Created by : Vial Joris
**
********************************************************************/

#include <iostream>
#include <sstream>
#include <algorithm>
#include "GraphicConfiguration.hpp"
#include "Log.hpp"
#include "AssetPath.h"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	////	Constructor / Destructor
	/////////////////////////////////////////////////////////////////////

	CGraphicConfiguration::CGraphicConfiguration(void)
	{
		_path = "";
	}

	CGraphicConfiguration::~CGraphicConfiguration(void)
	{
	}

	/////////////////////////////////////////////////////////////////////
	////	Set the config file path
	/////////////////////////////////////////////////////////////////////

	void CGraphicConfiguration::setConfigFilePath(const std::string &path)
	{
		_path = path;
	}

	/////////////////////////////////////////////////////////////////////
	////	Load the graphic configuration file
	/////////////////////////////////////////////////////////////////////

	void CGraphicConfiguration::fail(void)
	{
		VC_WARNING(std::string("Unable to load graphic configuration file") +
		", using default values");

		loadDefaultConfig();

		// Keep copy
		_configTemporary = _config;
	}

	void CGraphicConfiguration::loadConfig(void)
	{
		VC_INFO("Loading graphic configuration from " + _path);

		_buffer.clear();

		TiXmlDocument file(_path.c_str());

		// Try to load config file, if it failed, set default values
		if (!file.LoadFile())
		return fail();

		// Load config file
		TiXmlElement *e = file.FirstChildElement();
		if (e == NULL) return fail();

		if (!loadParam("WindowSizeX", e)) return fail();
		if (!loadParam("WindowSizeY", e)) return fail();
		if (!loadParam("Fullscreen", e)) return fail();
		if (!loadParam("Borderless", e)) return fail();

		e = e->NextSiblingElement();
		if (e == NULL) return fail();

		if (!loadParam("FpsLimit", e)) return fail();
		if (!loadParam("VSync", e)) return fail();
		if (!loadParam("FXAA", e)) return fail();
		if (!loadParam("Bloom", e)) return fail();
		if (!loadParam("BloomCoeff", e)) return fail();
		if (!loadParam("MotionBlur", e)) return fail();

		e = e->NextSiblingElement();
		if (e == NULL) return fail();

		if (!loadParam("MaxParticleSystem", e)) return fail();
		if (!loadParam("MaxPlayer", e)) return fail();
		if (!loadParam("MaxWall", e)) return fail();
		if (!loadParam("MaxSpeedfield", e)) return fail();
		if (!loadParam("MaxRespawn", e)) return fail();
		if (!loadParam("MaxFlag", e)) return fail();
		if (!loadParam("MaxBullet", e)) return fail();
		if (!loadParam("MaxBomb", e)) return fail();
		if (!loadParam("MaxRocket", e)) return fail();
		if (!loadParam("MaxTurret", e)) return fail();
		if (!loadParam("MaxGravityfield", e)) return fail();
		if (!loadParam("MaxBot", e)) return fail();
		if (!loadParam("MaxCapture", e)) return fail();

		e = e->NextSiblingElement();
		if (e == NULL) return fail();

		if (!loadParam("DisplayAim", e)) return fail();
		if (!loadParam("DisplayBackground", e)) return fail();
		if (!loadParam("DisplayCursor", e)) return fail();

		// Convert the string to value and store them into the
		// GraphicConfigurationAttribs structure
		convertValues();

		// Check if values are valid
		checkConfig();

		// Keep copy
		_configTemporary = _config;

		VC_INFO("Loading graphic configuration from " + _path + " succed");
	}

	bool CGraphicConfiguration::loadParam(const std::string &name, TiXmlElement *el)
	{
		const char *res = el->Attribute(name.c_str());

		// If the config file is corrupt, load default config
		if (res == NULL)
		{
			VC_WARNING(std::string("Unable to find attributes : ") + name +
			", ignoring config file");
			loadDefaultConfig();
			return false;
		}

		// Keep the string into the buffer
		_buffer[name] = std::string(res);

		return true;
	}

	/////////////////////////////////////////////////////////////////////
	////	Do the convertion operation after loading the config file
	/////////////////////////////////////////////////////////////////////

	void CGraphicConfiguration::convertValues(void)
	{
		auto it = _buffer.begin();
		auto end = _buffer.end();

		while (it != end)
		{
			if (it->first == "WindowSizeX")
			_config.winX = std::atoi(it->second.c_str());
			else if (it->first == "WindowSizeY")
			_config.winY = std::atoi(it->second.c_str());
			else if (it->first == "Fullscreen")
			_config.fullscreen = (bool)std::atoi(it->second.c_str());
			else if (it->first == "Borderless")
			_config.borderless = (bool)std::atoi(it->second.c_str());

			else if (it->first == "FpsLimit")
			_config.fpslimit = (int)std::atoi(it->second.c_str());
			else if (it->first == "VSync")
			_config.vsync = (bool)std::atoi(it->second.c_str());
			else if (it->first == "FXAA")
			_config.fxaa = (bool)std::atoi(it->second.c_str());
			else if (it->first == "Bloom")
			_config.bloom = (bool)std::atoi(it->second.c_str());
			else if (it->first == "BloomCoeff")
			_config.bloomCoeff = std::atof(it->second.c_str());
			else if (it->first == "MotionBlur")
			_config.motionBlur = (bool)std::atoi(it->second.c_str());

			else if (it->first == "MaxParticleSystem")
			_config.maxParticleSystem = std::atoi(it->second.c_str());
			else if (it->first == "MaxPlayer")
			_config.maxPlayer = std::atoi(it->second.c_str());
			else if (it->first == "MaxWall")
			_config.maxWall = std::atoi(it->second.c_str());
			else if (it->first == "MaxSpeedfield")
			_config.maxSpeedfield = std::atoi(it->second.c_str());
			else if (it->first == "MaxRespawn")
			_config.maxRespawn = std::atoi(it->second.c_str());
			else if (it->first == "MaxFlag")
			_config.maxFlag = std::atoi(it->second.c_str());
			else if (it->first == "MaxBullet")
			_config.maxBullet = std::atoi(it->second.c_str());
			else if (it->first == "MaxBomb")
			_config.maxBomb = std::atoi(it->second.c_str());
			else if (it->first == "MaxRocket")
			_config.maxRocket = std::atoi(it->second.c_str());
			else if (it->first == "MaxTurret")
			_config.maxTurret = std::atoi(it->second.c_str());
			else if (it->first == "MaxGravityfield")
			_config.maxGravityfield = std::atoi(it->second.c_str());
			else if (it->first == "MaxBot")
			_config.maxBot = std::atoi(it->second.c_str());
			else if (it->first == "MaxCapture")
			_config.maxCapture = std::atoi(it->second.c_str());

			else if (it->first == "DisplayAim")
			_config.displayaim = (bool)std::atoi(it->second.c_str());
			else if (it->first == "DisplayBackground")
			_config.displayBackground = (bool)std::atoi(it->second.c_str());
			else if (it->first == "DisplayCursor")
			_config.displayCursor = (bool)std::atoi(it->second.c_str());

			++it;
		}
	}

	/////////////////////////////////////////////////////////////////////
	////	Check current configuration
	/////////////////////////////////////////////////////////////////////

	std::vector<std::pair<unsigned int, unsigned int> > CGraphicConfiguration::getResolutions(void)
	{
		std::vector<std::pair<unsigned int, unsigned int> > resolutions;

		// Get video modes
		int count;
		const GLFWvidmode* modes = glfwGetVideoModes(getPrimaryMonitor(), &count);

		for (int i = 0; i < count; ++i)
		{
			const GLFWvidmode &current = modes[i];
			resolutions.push_back(std::pair<unsigned int, unsigned int>(current.width, current.height));
		}

		return resolutions;
	}

	void CGraphicConfiguration::checkConfig(void)
	{
		if (_config.borderless == true)
		{
			// Get the current resolution
			const GLFWvidmode* mode = glfwGetVideoMode(getPrimaryMonitor());

			_config.winX = mode->width;
			_config.winY = mode->height;
		}
		else
		{
			_config.winX = std::max(640, _config.winX);
			_config.winY = std::max(480, _config.winY);
		}
	}

	/////////////////////////////////////////////////////////////////////
	////	Load the default config
	/////////////////////////////////////////////////////////////////////

	void CGraphicConfiguration::loadDefaultConfig(void)
	{
		VC_WARNING("Failed to load GraphicConfig.xml, loading default value");

		// Get the current resolution
		const GLFWvidmode* mode = glfwGetVideoMode(getPrimaryMonitor());

		_config.winX = mode->width;
		_config.winY = mode->height;
		_config.fullscreen = true;
		_config.borderless = false;

		_config.vsync = true;
		_config.fxaa = false;
		_config.bloom = false;
		_config.motionBlur = false;
		_config.bloomCoeff = 0;
		_config.fpslimit = -1;

		_config.maxParticleSystem = 250;
		_config.maxPlayer = 32;
		_config.maxWall = 100;
		_config.maxSpeedfield = 100;
		_config.maxRespawn = 10;
		_config.maxFlag = 10;
		_config.maxBullet = 1000;
		_config.maxBomb = 1000;
		_config.maxRocket = 1000;
		_config.maxTurret = 100;
		_config.maxGravityfield = 50;
		_config.maxBot = 250;
		_config.maxCapture = 100;

		_config.displayaim = true;
		_config.displayBackground = true;
		_config.displayCursor = true;
	}

	/////////////////////////////////////////////////////////////////////
	////	Print out the current graphic configuration
	/////////////////////////////////////////////////////////////////////

	void CGraphicConfiguration::dumpConfig(void)
	{
		std::stringstream ss("");

		ss << std::endl << "<< -------- Current graphic configuration -------- >>" << std::endl;
		ss << "--> WindowSizeX [" << _config.winX << "]" << std::endl;
		ss << "--> WindowSizeY [" << _config.winY << "]" << std::endl;
		ss << "--> Fullscreen [" << _config.fullscreen << "]" << std::endl;
		ss << "--> Borderless [" << _config.borderless << "]" << std::endl;
		ss << "--> FpsLimit [" << _config.fpslimit << "]" << std::endl;
		ss << "--> VSync [" << _config.vsync << "]" << std::endl;
		ss << "--> FXAA [" << _config.fxaa << "]" << std::endl;
		ss << "--> Bloom [" << _config.bloom << "]" << std::endl;
		ss << "--> BloomCoeff [" << _config.bloomCoeff << "]" << std::endl;
		ss << "--> MotionBlur [" << _config.motionBlur << "]" << std::endl;
		ss << "--> MaxParticleSystem [" << _config.maxParticleSystem << "]" << std::endl;
		ss << "--> MaxPlayer [" << _config.maxPlayer << "]" << std::endl;
		ss << "--> MaxWall [" << _config.maxWall << "]" << std::endl;
		ss << "--> MaxSpeedfield [" << _config.maxSpeedfield << "]" << std::endl;
		ss << "--> MaxRespawn [" << _config.maxRespawn << "]" << std::endl;
		ss << "--> MaxFlag [" << _config.maxFlag << "]" << std::endl;
		ss << "--> MaxBullet [" << _config.maxBullet << "]" << std::endl;
		ss << "--> MaxBomb [" << _config.maxBomb << "]" << std::endl;
		ss << "--> MaxRocket [" << _config.maxRocket << "]" << std::endl;
		ss << "--> MaxTurret [" << _config.maxTurret << "]" << std::endl;
		ss << "--> MaxGravityfield [" << _config.maxGravityfield << "]" << std::endl;
		ss << "--> MaxBot [" << _config.maxBot << "]" << std::endl;
		ss << "--> MaxCapture [" << _config.maxCapture << "]" << std::endl;
		ss << "--> DisplayAim [" << _config.displayaim << "]" << std::endl;
		ss << "--> DisplayBackground [" << _config.displayBackground << "]" << std::endl;
		ss << "--> DisplayCursor [" << _config.displayCursor << "]" << std::endl;
		ss << "<< ----------------------------------------------- >>" << std::endl;

		VC_INFO_CRITICAL(ss.str());
	}

	/////////////////////////////////////////////////////////////////////
	////	Return the primary monitor
	/////////////////////////////////////////////////////////////////////

	GLFWmonitor *CGraphicConfiguration::getPrimaryMonitor(void)
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		if (primary == NULL)
		VC_ERROR_CRITICAL("CGraphicConfiguration::unable to get primary monitor !");

		return primary;
	}

	/////////////////////////////////////////////////////////////////////
	////	Return the current configuration
	////	Don't use this function, just include "GraphicConfiguration.hpp"
	////	and use the global function "GCONFIG"
	////	For example : GCONFIG.fullscreen
	/////////////////////////////////////////////////////////////////////

	GraphicConfigurationAttribs& CGraphicConfiguration::getConfig(void)
	{
		return _config;
	}

	GraphicConfigurationAttribs& CGraphicConfiguration::getConfigTemporary(void)
	{
		return _configTemporary;
	}

	/////////////////////////////////////////////////////////////////////
	////	Save the current configuration into GraphicConfig.xml
	/////////////////////////////////////////////////////////////////////

	void CGraphicConfiguration::saveCurrentConfiguration(void)
	{
		TiXmlDocument doc;
		TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "utf-8", "");
		doc.LinkEndChild(decl);

		// Window attributes
		TiXmlElement *window = new TiXmlElement("window");
		window->SetAttribute("WindowSizeX", _configTemporary.winX);
		window->SetAttribute("WindowSizeY", _configTemporary.winY);
		window->SetAttribute("Fullscreen", _configTemporary.fullscreen);
		window->SetAttribute("Borderless", _configTemporary.borderless);

		// PostFX
		TiXmlElement *postfx = new TiXmlElement("postfx");
		postfx->SetAttribute("FpsLimit", _configTemporary.fpslimit);
		postfx->SetAttribute("VSync", _configTemporary.vsync);
		postfx->SetAttribute("FXAA", _configTemporary.fxaa);
		postfx->SetAttribute("Bloom", _configTemporary.bloom);
		postfx->SetAttribute("BloomCoeff", _configTemporary.bloomCoeff);
		postfx->SetAttribute("MotionBlur", _configTemporary.bloom);

		// MaxEntities
		TiXmlElement *maxentities = new TiXmlElement("maxentities");
		maxentities->SetAttribute("MaxParticleSystem", _configTemporary.maxParticleSystem);
		maxentities->SetAttribute("MaxPlayer", _configTemporary.maxPlayer);
		maxentities->SetAttribute("MaxWall", _configTemporary.maxWall);
		maxentities->SetAttribute("MaxSpeedfield", _configTemporary.maxSpeedfield);
		maxentities->SetAttribute("MaxRespawn", _configTemporary.maxRespawn);
		maxentities->SetAttribute("MaxFlag", _configTemporary.maxFlag);
		maxentities->SetAttribute("MaxBullet", _configTemporary.maxBullet);
		maxentities->SetAttribute("MaxBomb", _configTemporary.maxBomb);
		maxentities->SetAttribute("MaxRocket", _configTemporary.maxRespawn);
		maxentities->SetAttribute("MaxTurret", _configTemporary.maxTurret);
		maxentities->SetAttribute("MaxGravityfield", _configTemporary.maxGravityfield);
		maxentities->SetAttribute("MaxBot", _configTemporary.maxBot);
		maxentities->SetAttribute("MaxCapture", _configTemporary.maxCapture);

		// Hud
		TiXmlElement *hud = new TiXmlElement("hud");
		hud->SetAttribute("DisplayAim", _configTemporary.displayaim);
		hud->SetAttribute("DisplayBackground", _configTemporary.displayBackground);
		hud->SetAttribute("DisplayCursor", _configTemporary.displayCursor);

		doc.LinkEndChild(window);
		doc.LinkEndChild(postfx);
		doc.LinkEndChild(maxentities);
		doc.LinkEndChild(hud);

		std::string path(ASSETS_PATH + "GraphicConfig.xml");
		doc.SaveFile(path.c_str());
	}
}
