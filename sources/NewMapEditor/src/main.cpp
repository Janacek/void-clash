//
// main.cpp for Void Clash in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Fri Aug  2 10:43:43 2013 brunel guillaume
// Last update Wed Mar 19 16:26:43 2014 sadin_n
//

#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include "main.hpp"
#include "Manager.hpp"
#include "MapParser.hpp"
#include "Log.hpp"

// Globals
extern bool	G_isOffline;
extern bool	G_isServer;
extern bool G_isMapEditor;
extern std::string G_ip;

int init_game()
{
	try
	{
		std::srand(time(NULL));
		Manager manager;
		manager.start();
		manager.run();
	}
	catch (std::runtime_error &error)
	{
		std::cerr << "Runtime Error encountered ! What : " << error.what() << std::endl;

		// Don't delete the voidclash log file (for bug report)
		S_Log->warningCritical("RUNTIME_ERROR: " + std::string(error.what()));
		S_Log->stop(false);

		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	std::srand(time(NULL));

	G_isServer = false;
	G_isMapEditor = true;
	G_isOffline = true;
	return (init_game());
}
