//
// Manager.cpp for toto in /home/vial_j/travail/pfa/client
// 
// Made by vial_j
// Login   <vial_j@epitech.net>
// 
// Started on  Tue Nov  5 22:35:17 2013 vial_j
// Last update Wed Mar 19 13:39:37 2014 sadin_n
//

#include <cstring>
#include <stdexcept>
#include <iostream>
#include "Event.hpp"
#include "Manager.hpp"
#include "Defines.h"
#include "ConfigParser.hpp"
#include "Sender.hpp"
#include "HudRessources.hpp"


// Globals
extern int sizeX;
extern int sizeY;
extern int volume;
extern t_config *_conf;
extern std::string g_ip;
extern bool G_isOffline;
extern bool G_isMapEditor;

Manager::Manager() :
_gameEngine(NULL), _graphicEngine(NULL), _soundEngine(NULL),
_inputEngine(NULL), _physicEngine(NULL), _guiEngine(NULL)
{
	_gameState = RUN;
}

Manager::~Manager()
{
	delete _gameEngine;
	delete _graphicEngine;
	delete _soundEngine;
	delete _inputEngine;
	delete _physicEngine;
	delete _guiEngine;
}

//////////////////////////////////////////////////////////////////////
/////	Instantiate / initialize / start and stock all engines
//////////////////////////////////////////////////////////////////////

void Manager::start(void)
{
	Event::getMainEventList();

	_gameEngine = new GameEngine();
	_physicEngine = new PhysicEngine();
	_graphicEngine = new GraphicEngine();
	_soundEngine = new SoundEngine();
	_inputEngine = new InputEngine();
	_guiEngine = new GUIEngine();

	// GameEngine
	_gameEngine->start();

	if (G_isOffline)
	{
		S_Map->addPlayer(std::make_shared<Player>(0, 0, 0, 0, true), 1);
	}

	// PhysicEngine
	_physicEngine->start();

	// GraphicEngine
	_graphicEngine->start();

	//SoundENgine
	_soundEngine->start();

	// InputEngine
	_inputEngine->start();

	// GUIEngine
	_guiEngine->start();

	// Set the FPS Limit in MapUtils
	S_Map->setFpsLimit(GCONFIG.fpslimit);

	if (G_isMapEditor == false)
	{
		GUIManager::Instance().initCegui();
	}
}

//////////////////////////////////////////////////////////////////////
/////	Main program loop
//////////////////////////////////////////////////////////////////////

void Manager::run(void)
{
	_soundEngine->playInGameMusic();

	while (42)
	{
		// Save mouse position for this frame
		HudRessources::getInstance()->setMousePosition(sf::Mouse::getPosition(S_Context->getWindow()));

		// Get inputs
		if (_inputEngine->update(S_Map->getDeltaTime()) == EXIT)
			return stop();
		// Sleep & update delta time & time related events
		S_Map->update();
		// Update GUI
		if (_guiEngine->update(S_Map->getDeltaTime(), true) == EXIT)
			return stop();


		// Physic Engine
		_physicEngine->update(S_Map->getDeltaTime());

		// Update camera needed by InputEngine for world mouse position
		_graphicEngine->updateCamera();





		// Game Engine
		if (_gameEngine->update(S_Map->getDeltaTime()) == EXIT)
			return stop();

		S_Map->addNewObjects();

		// SoundEngine
		if (_soundEngine->update(S_Map->getDeltaTime()) == EXIT)
			return stop();

		// Update followed player
		S_Map->setPlayerFollowed();


		// GraphicEngine
		if (_graphicEngine->update(S_Map->getDeltaTime()) == EXIT)
			return stop();

		S_Map->deleteObjects();

		/* Reboot graphic engine ? */
		auto events = Event::getEventByType(ev_REBOOT_GRAPHIC);
		if (events != NULL)
			_graphicEngine->reboot();





		// Update GUI
		if (_guiEngine->update(S_Map->getDeltaTime(), false) == EXIT)
			return stop();





		// Clear events
		Event::clearEvents();
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
	_graphicEngine->stop();
	_soundEngine->stop();
	_inputEngine->stop();
	_guiEngine->stop();
}
