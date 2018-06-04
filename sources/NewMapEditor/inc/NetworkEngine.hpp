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

// Debug level
// 0 = NONE
// 1 = ERROR
// 2 = ERROR + IMPORTANT EVENTS 
// 3 = ALL

# define        LOG_LEVEL 2

enum	ePacketType
  {
	PACKET_NONE = 42,
	PACKET_CONFIRMATION,
	PACKET_INPUT,
	PACKET_UPDATE,
	PACKET_WELCOME,
	PACKET_SYNCHRO,
	PACKET_CONFIG,
	PACKET_EVENT_LAUNCHED,
	PACKET_EVENT_CHAT,
	PACKET_EVENT_KILL,
	PACKET_EVENT_PLAYER_HIT,
	PACKET_EVENT_JOIN_TEAM,
	PACKET_EVENT_IDENT,
	PACKET_EVENT_SWITCH_MAP_MODE,
	PACKET_EVENT_CAPTURE_FLAG,
	PACKET_EVENT_KICK_PLAYER,
	PACKET_EVENT_TURRET_DESTROYED,
	PACKET_PING,
	PACKET_WEAPON_SELECTION,
	PACKET_RESET_ROUND,				// Server -> client - NONE
	PACKET_MAP_LIST,				// Server to client - list std::string mapname
	PACKET_REQUEST_CHANGE_MAP,		// Client -> server - std::string mapname
	PACKET_REQUEST_CHANGE_MODE,		// Client -> server - enum mode
	PACKET_REQUEST_PLAYER_KICK,		// Client -> server - Player ID
	PACKET_REQUEST_HORDE_ACTIVATE,	// Client -> server - NONE
	PACKET_REQUEST_HORDE_DESACTIVATE,	// Client -> server - NONE
	PACKET_EVENT_ADD,				// Server -> client - Subpacket of update / welcome and sync
	// Weapon: obj->getType() << obj->getId() << templatedObj->getOwner()->getId() << *templatedObj << templatedObj->getWeaponIndex();
	// Other: obj->getType() << obj->getId() << *templatedObj;
	PACKET_EVENT_DELETE				// Server -> client - Subpacket of update / welcome and sync
	// object id
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
