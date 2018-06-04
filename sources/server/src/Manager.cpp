//
// Manager.cpp for toto in /home/vial_j/travail/pfa/client
// 
// Made by vial_j
// Login   <vial_j@epitech.net>
// 
// Started on  Tue Nov  5 22:35:17 2013 vial_j
// Last update Mon Feb 24 18:27:50 2014 Gegon
//

#include <stdio.h>
#include	<cstring>
#include	<stdexcept>
#include	<iostream>
#include	<signal.h>
#include	"Manager.hpp"
#include	"Defines.h"
#include	"Log.hpp"
#include	"Map.hpp"

extern std::string G_ip;
extern int sizeX;
extern int sizeY;

// Catch event funcs
volatile bool		G_isRunning;
#if defined (_WIN32)
#include <windows.h>
#else
#endif

#if defined (_WIN32)
BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
		G_isRunning = false;
		// Signal is handled - don't pass it on to the next handler
		return TRUE;
	default:
		// Pass signal on to the next handler
		return FALSE;
	}
}

#else
void intHandler(int dummy) {
	sf::sleep(sf::seconds(1));
	G_isRunning = false;
}
#endif


Manager::Manager() :
_gameEngine(NULL), _physicEngine(NULL)
{
	G_isRunning = false;
}

Manager::~Manager()
{
	delete _gameEngine;
	delete _physicEngine;
	delete _networkEngine;
}

//////////////////////////////////////////////////////////////////////
/////	Instantiate / initialize / start and stock all engines
//////////////////////////////////////////////////////////////////////

void	Manager::start()
{
	G_isRunning = true;
#if defined (_WIN32)
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
#else
	signal(SIGINT, intHandler);
	signal(SIGQUIT, intHandler);
#endif

	// Open the log file
	S_Log->start(DEBUG_LEVEL, true);
	VC_INFO_CRITICAL("Server is starting...");

	Event::getMainEventList();

	_gameEngine = new GameEngine();
	_physicEngine = new PhysicEngine();
	_gameEngine->start();
	_physicEngine->start();

	S_Map->addNewObjects();
	Event::clearEvents();

	// NetworkEngine
	_networkEngine = new NetworkEngine();
	_networkEngine->start();

	// Set FPS limit
	S_Map->setFpsLimit(SERVER_TICKRATE);
}

//////////////////////////////////////////////////////////////////////
/////	Main program loop
//////////////////////////////////////////////////////////////////////

void		Manager::run(void)
{
	VC_INFO_CRITICAL("-- Server is running. Use ctrl-C to stop it. --");
	while (G_isRunning)
	{
		// Sleep & update delta time & time related events
		S_Map->update();

		// Receive packets from clients
		_networkEngine->getReceiver()->update();

		// Physic Engine
		_physicEngine->update(S_Map->getDeltaTime());

		// Game Engine
		if (_gameEngine->update(S_Map->getDeltaTime()) == EXIT)
			return stop();

		// Network
		//_networkEngine->update();

		// Send packets (update / events) to clients
		_networkEngine->getSender()->update();

		S_Map->addNewObjects();
		S_Map->deleteObjects();

		// Clear events
		Event::clearEvents();
	}
}

//////////////////////////////////////////////////////////////////////
/////	Stop all engine and free ressources
//////////////////////////////////////////////////////////////////////

void	Manager::stop(void)
{
	S_Log->info("Server is closing...");

	// Kick all players
	auto it = S_Map->getPlayers()->begin();
	auto end = S_Map->getPlayers()->end();
	while (it != end)
	{
		_networkEngine->getSender()->sendPacketEvent(PACKET_EVENT_KICK_PLAYER, (*it)->getId());
		++it;
	}

	// Stop the engine + send stop to web server
	_gameEngine->stop();
	_physicEngine->stop();
	_networkEngine->stop();
}
