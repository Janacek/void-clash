#ifndef NETWORKENGINE_HPP_
# define NETWORKENGINE_HPP_

class	Manager;
class	ClientHandle;

// Debug level
// 0 = NONE
// 1 = ERROR
// 2 = ERROR + IMPORTANT EVENTS
// 3 = ALL

# define	LOG_LEVEL 2

// When no packet received from client, is considered as disconnect after INACTIVITY_TIMEOUT sec
#define		RESPONSIVE_TIMEOUT 5.f // Do not send packets if no reply after RESPONSIVE_TIMEOUT from client (removed when received a packet from client)

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

#include	<SFML/Network.hpp>
#include	<list>
#include	"Sender.hpp"
#include	"Receiver.hpp"
#include	"Command.hpp"
#include	"WebSender.hpp"

class NetworkEngine
{
public:
  NetworkEngine();
  ~NetworkEngine();

  bool	start();
  void	stop();
  void	update();

  void	addNewClient(const sf::IpAddress & ip, unsigned short port);

  // Getterz
  std::list<ClientHandle *>		&getClients();
  sf::UdpSocket				*getSocket();
  Receiver				*getReceiver();		// Receive from clients
  Sender				*getSender();		// Send infos to clients
  WebSender				*getWebSender();	// Communicate with central server

  // Debug funcs
  void		printClientsInfo();
  sf::Uint32	getNbClient() const;


  void	setHost(const char *host);

  void	pingClients();

  // Log
  void		printLog(int level, const std::string &msg, const char *color = NULL);
  void		printLogWithId(int level, const std::string &msg, const sf::Uint32 &id, const char *color = NULL);

  // Find
  std::shared_ptr<AObject>	findObjectWithID(sf::Uint32 id);
  std::shared_ptr<Player>	findPlayerWithIP(sf::IpAddress ip, unsigned short port);
  ClientHandle				*findClientHandleWithIP(sf::IpAddress ip, unsigned short port);
  ClientHandle				*findClientHandleWithID(sf::Uint32 id);

private:
  std::string		_serverIp;
  unsigned short	_serverPort;

  sf::UdpSocket			_serverSocket;
  std::list<ClientHandle *>	_clients;

  Sender			*_sender;		// Send infos to clients
  Receiver			*_receiver;		// Receive from clients
  WebSender			*_webSender;	// Communicate with central server
};

#endif
