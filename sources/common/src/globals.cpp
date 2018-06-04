#include "ConfigParser.hpp"
#include "Defines.h"

int	sizeX = 0;
int	sizeY = 0;
int	volume = 100;
t_config *G_conf = NULL;
bool G_isServer = false;
bool G_isOffline = false;
int G_id = 1;
std::string G_ip = "0";
int G_port = BIND_PORT_START;

// Handle different path for ConfigFiles (due to severals instances on the same server)
#ifdef _WIN32
	std::string G_configPath = "..\\";
#else
	std::string G_configPath = "../";
#endif
