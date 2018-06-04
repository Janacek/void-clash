/********************************************************************
**
**              Manager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <cstring>
#include <stdexcept>
#include <iostream>
#include "Event.hpp"
#include "Manager.hpp"
#include "Defines.h"
#include "ConfigParser.hpp"
#include "Sender.hpp"
#include "HudRessources.hpp"
#include "Log.hpp"
#include "GraphicConfiguration.hpp"
#include "GUIManager.hpp"

// Globals
extern int sizeX;
extern int sizeY;
extern int volume;
extern t_config *G_conf;
extern std::string G_ip;
extern int G_port;
extern bool G_isOffline;

Manager::Manager()
{
	_gameState = RUN;

	_gameEngine = NULL;
	_graphicEngine = NULL;
	_soundEngine = NULL;
	_inputEngine = NULL;
	_physicEngine = NULL;
}

Manager::~Manager()
{
	delete _gameEngine;
	delete _graphicEngine;
	delete _soundEngine;
	delete _inputEngine;
	delete _physicEngine;
	delete _networkEngine;
}

//////////////////////////////////////////////////////////////////////
/////	Instantiate / initialize / start and stock all engines
//////////////////////////////////////////////////////////////////////

void Manager::start(void)
{
	// Open the log file
	S_Log->start(DEBUG_LEVEL, true);

	Event::getMainEventList();

	_gameEngine = new GameEngine();
	_physicEngine = new PhysicEngine();
	_soundEngine = new SoundEngine_();
	_inputEngine = new InputEngine_();
	_networkEngine = new NetworkEngine();
	_graphicEngine = new VoidClashGraphics::GraphicEngine();

	// GameEngine
	_gameEngine->start();

	// PhysicEngine
	_physicEngine->start();

	// GraphicEngine
	_graphicEngine->start();

	// SoundENgine
	_soundEngine->start();

	 //InputEngine
	_inputEngine->start();

	// NetworkEngine
	if (!G_isOffline)
	{
		_networkEngine->setHost(G_ip.c_str());
		_networkEngine->setPort(G_port);
		_networkEngine->start();
	}

	// Set the FPS Limit in MapUtils
	S_Map->setFpsLimit(GCONFIG.fpslimit);
}

//////////////////////////////////////////////////////////////////////
/////	Main program loop
//////////////////////////////////////////////////////////////////////

void Manager::run(void)
{
	bool shouldQuit = false;

	//_soundEngine->playInGameMusic();

	while (42)
	{
		// Flush delayed event sent by the GUIManager
		S_GUI->flushDelayedEvent();

		// Reboot graphic engine ?
		auto events = Event::getEventByType(ev_REBOOT_GRAPHIC);
		if (events != NULL)
			_graphicEngine->reboot();

		// Get inputs
		if (_inputEngine->update(S_Map->getDeltaTime()) == EXIT)
			shouldQuit = true;

		// Sleep & update delta time & time related events
		S_Map->update();

		// Physic Engine
		_physicEngine->update(S_Map->getDeltaTime());

		// Network engine - ping & send & receive
		if (!G_isOffline)
		{
			_networkEngine->getSender()->update();
			if (_networkEngine->getReceiver()->update() == false)
				shouldQuit = true;
		}

		// Game Engine
		if (_gameEngine->update(S_Map->getDeltaTime()) == EXIT)
			shouldQuit = true;

		// Update object list
		S_Map->addNewObjects();

		// SoundEngine
		if (_soundEngine->update(S_Map->getDeltaTime()) == EXIT)
			shouldQuit = true;

		// Update followed player
		S_Map->setPlayerFollowed();

		// GraphicEngine - Don't update it if we change GraphicConfiguration
		if (_graphicEngine->update(S_Map->getDeltaTime()) == EXIT)
			shouldQuit = true;

		// Update GUI
		if (events == NULL)
			S_GUI->update(S_Map->getDeltaTime().asSeconds());

		// Update object list
		S_Map->deleteObjects();

		// Clear events
		Event::clearEvents();

		// Exit if needed
		if (shouldQuit)
		{
			stop();
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////
/////	Stop all engine and free ressources
//////////////////////////////////////////////////////////////////////

void Manager::stop(void)
{
	ConfigParser *parse = new ConfigParser;
	delete parse;

	_gameEngine->stop();
	_physicEngine->stop();
	_soundEngine->stop();
	_inputEngine->stop();
	_networkEngine->stop();
	_graphicEngine->stop();
	S_Log->stop();
}
