/********************************************************************
**
**              GraphicConfiguration.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOIDCLASH_GRAPHIC_CONFIGURATION
#define VOIDCLASH_GRAPHIC_CONFIGURATION

#include <map>
#include <string>
#include <vector>
// Fix linking bugs on Linux
#ifndef _WIN32
#define TIXML_USE_STL
#endif
#include <tinyxml.h>
#include "VoidClash_OpenGL.h"
#include "SingletonBase.hpp"

namespace VoidClashGraphics
{
	struct GraphicConfigurationAttribs
	{
		// Window size
		int winX;
		int winY;
		bool fullscreen;
		bool borderless;

		// VSync and PostFX
		int fpslimit;
		bool vsync;
		bool fxaa;
		bool bloom;
		bool motionBlur;
		float bloomCoeff;

		// Modules definitions
		unsigned int maxParticleSystem;
		unsigned int maxPlayer;
		unsigned int maxWall;
		unsigned int maxSpeedfield;
		unsigned int maxRespawn;
		unsigned int maxFlag;
		unsigned int maxBullet;
		unsigned int maxBomb;
		unsigned int maxRocket;
		unsigned int maxTurret;
		unsigned int maxGravityfield;
		unsigned int maxBot;
		unsigned int maxCapture;

		// Hud
		bool displayaim;
		bool displayBackground;
		bool displayCursor;
	};

	/////////////////////////////////////////////////////////////////////
	////	This class manage the graphic engine configuration
	/////////////////////////////////////////////////////////////////////

	class CGraphicConfiguration
	{
	public:
		CGraphicConfiguration(void);
		~CGraphicConfiguration(void);

		void start(void) {};
		void loadConfig(void);
		void setConfigFilePath(const std::string &path);
		void dumpConfig(void);
		GraphicConfigurationAttribs& getConfig(void);
		GraphicConfigurationAttribs& getConfigTemporary(void);
		std::vector<std::pair<unsigned int, unsigned int> > getResolutions(void);

		void saveCurrentConfiguration(void);

	private:
		bool loadParam(const std::string &name, TiXmlElement *el);
		void checkConfig(void);
		void convertValues(void);
		void loadDefaultConfig(void);
		void fail(void);

		// Get the primary monitor
		GLFWmonitor* getPrimaryMonitor(void);

	private:
		std::string _path;
		GraphicConfigurationAttribs _config;
		GraphicConfigurationAttribs _configTemporary;
		std::map<std::string, std::string> _buffer;
	};
}

typedef SingletonBase<VoidClashGraphics::CGraphicConfiguration>
	GraphicConfigurationSingleton;

// Shortcut
#define S_Config GraphicConfigurationSingleton::self()
#define GCONFIG S_Config->getConfig()
#define GCONFIG_TMP S_Config->getConfigTemporary()

#endif /* end of include guard: VOIDCLASH_GRAPHIC_CONFIGURATION */
