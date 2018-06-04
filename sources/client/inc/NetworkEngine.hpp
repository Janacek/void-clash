//
// NetworkEngine.cpp for pfa in /home/volent/dev/pfa/client/NetworkEngine
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Thu Sep 19 15:38:20 2013 florent espanet
// Last update Mon Feb 24 17:20:56 2014 Gegon
//

#ifndef		NETWORKENGINE_HPP_
# define	NETWORKENGINE_HPP_

#include	<SFML/Network.hpp>
#include	"AEngine.hpp"
#include	"Player.hpp"
#include	"Receiver.hpp"

class Sender;

# define		LAGGY_PACKET_THRESHOLD 100.f			// Packet deleted if it has more than this defined delay compared to previous packets
# define		NB_OF_PACKET_FOR_AVERAGE_LATENCY	100.f	

enum	ePacketType
{
	PACKET_NONE = 42,

	//................. Start
	PACKET_VERSION_CHECK,		// Server -> client - std::string
	PACKET_EVENT_IDENT,			// Server -> client - player id
	PACKET_CONFIRMATION,		// Client -> server - packet id (Client confirm he the packet)
	PACKET_WELCOME,				// Server -> client - loop on all game object and serialize them (heavy - only at start)
	PACKET_CONFIG,				// Server -> client - t_conf

	//................. Low level sync
	PACKET_INPUT,				// Client -> server - t_actions
	PACKET_UPDATE,				// Server -> client - loop on all game object and serialize them (light - every tickrate)
	PACKET_SYNCHRO,				// Server -> client - loop on all game object and serialize them (medium - every sec)
	PACKET_EVENT_ADD,			// Server -> client - Subpacket of update / welcome and sync
	PACKET_EVENT_DELETE,		// Server -> client - Subpacket of update / welcome and sync

	//................. General
	PACKET_DISPLAY_STRING,			// Server -> client - int (type of display) / std::string
	PACKET_EVENT_CHAT,				// Client -> server - std::string
	PACKET_EVENT_JOIN_TEAM,			// Client -> server - team id
	PACKET_NICKNAME_CHANGE,			// Client -> server - std::string
	PACKET_PING,
	PACKET_WEAPON_SELECTION,		// Client -> server
	PACKET_RESET_ROUND,				// Server -> client - NONE
	PACKET_GAME_START,				// Server -> client - NONE
	PACKET_MAP_LIST,				// Server ->client - list std::string mapname
	PACKET_EVENT_SERVER_FULL,		// Server -> client
	PACKET_EVENT_SWITCH_MAP_MODE,	// Server -> client
	PACKET_EVENT_KICK_PLAYER,		// Server -> client
	PACKET_EVENT_QUIT,				// Client -> server

	//................. Gameplay related
	PACKET_EVENT_KILL,			// Server -> clients - Killer / Killed
	PACKET_EVENT_PLAYER_HIT,	// Server -> clients - Hitter / Hilled
	PACKET_EVENT_LAUNCHED,		// Client -> serv and serv -> clients - AObject * - inform that a bomb has been launched
	PACKET_EVENT_CAPTURE_FLAG,	// Server -> clients - flag id / player id
	PACKET_EVENT_TURRET_DESTROYED,	// Server -> clients - AObject *

	//................. Server management
	PACKET_REQUEST_CHANGE_MAP,		// Client -> server - std::string mapname
	PACKET_REQUEST_CHANGE_MODE,		// Client -> server - enum mode
	PACKET_REQUEST_PLAYER_KICK,		// Client -> server - Player ID
	PACKET_REQUEST_HORDE_ACTIVATE,	// Client -> server - NONE
	PACKET_REQUEST_HORDE_DESACTIVATE	// Client -> server - NONE

};

class	Manager;

class	NetworkEngine : public AEngine
{
public:
  NetworkEngine();
  virtual ~NetworkEngine();

  virtual void start(void);
  virtual void stop(void);
  virtual eGameState update(const sf::Time &deltaTime);

  sf::UdpSocket	&getSocket();
  //sf::Uint32	getPublicIp();
  //sf::Uint32	getLocalIp();
  Receiver	*getReceiver();
  Sender	*getSender();
  std::string	&getServerIp();
  unsigned short getServerPort();

  void setHost(const char *host);
  void setPort(const int &port);

  void		printLog(int level, const std::string &msg, const char *color = NULL);
  void		printLogWithId(int level, const std::string &msg, const sf::Uint32 &id, const char *color = NULL);
private:
  sf::UdpSocket		_socket;
  std::string		_serverIp;
  unsigned short	_serverPort;

  //sf::Uint32		_publicIp;
  //sf::Uint32		_localIp;

  Receiver		*_receiver;
  Sender		*_sender;
};

#endif
