//
// main.cpp for Void Clash in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Fri Aug  2 10:43:43 2013 brunel guillaume
// Last update Tue Jan 14 23:01:20 2014 Gegon
//

#include	<cstdlib>
#include	<time.h>
#include	<iostream>
#include	<stdexcept>
#include	"main.hpp"
#include	"Manager.hpp"
#include	"Log.hpp"
#include	"Parser.hpp"
#include	"ConfigParser.hpp"
#include	"MapParser.hpp"

// Globals


extern int	sizeX;
extern int	sizeY;
extern bool	G_isOffline;
extern std::string G_ip;
extern t_config *G_conf;
extern bool G_isServer;
// Handle different path for ConfigFiles (due to severals instances on the same server)
extern std::string G_configPath;

int		init_game()
{
	try
	{
		Manager manager;
		manager.start();
		manager.run();
		manager.stop();
		VC_INFO_CRITICAL("Exiting...");
		sf::sleep(sf::seconds(1)); // Sleep to send to send close request
		S_Log->stop();
	}
	catch (std::runtime_error error)
	{
		std::cerr << "Runtime Error encountered ! What : " << error.what() << std::endl;

		// Don't delete the voidclash log file (for bug report)
		S_Log->warningCritical("RUNTIME_ERROR: " + std::string(error.what()));
		S_Log->stop(false);

		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// TODO : Real argument parsing :)
int		main(int ac, char **av)
{
	G_isServer = true;
	G_isOffline = false;

	if (ac >= 3)
	{
		sizeX = atoi(av[1]);
		sizeY = atoi(av[2]);
	}
	else
	{
		sizeX = 0;
		sizeY = 0;
	}

	// Handle different path for ConfigFiles (due to severals instances on the same server)
	if (ac == 2)
	{
		G_configPath = std::string(av[1]);
	}
	return (init_game());
}
