//
// Client.hpp for Void Clash in /home/gegon/pfa/server/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sat Sep 21 10:50:50 2013 Gegon
// Last update Wed Dec 25 21:05:06 2013 Gegon
//

#ifndef		CLIENT_HPP_
# define	CLIENT_HPP_

#include	<SFML/Network.hpp>

class	Player;

class	NetworkEngine;

class	ClientHandle
{
public:
  ClientHandle(NetworkEngine *networkEngine, sf::IpAddress ip, unsigned short port);
  ~ClientHandle();
  
  bool	ping();
  void	pong(sf::Packet &packet);

  void		setActions(sf::Packet &);
  void		resetActions();
  
  const sf::IpAddress	&getIp() const;
  unsigned short getPort() const;
  std::shared_ptr<Player>	getPlayer() const;

  // Activity
  void	active(sf::Time time);
  bool	isActive(sf::Time time); // True if server got a client reply since less than DECONNECTION_TIMEOUT
  bool	isResponsive(sf::Time time); // True if server got a client reply since less than INACTIVITY_TIMEOUT

  void	setStarted(bool started);

  void	setPort(unsigned short port);

  // Admin
  bool	isAdmin();
  void	admin();
private:
  sf::Clock		_clock;

  sf::Time		_lastActivity;
  float			_lastUpdateInput;

  sf::IpAddress		_ip;
  unsigned short	_port;
  
  std::shared_ptr<Player>	_player;

  NetworkEngine		*_networkEngine;

  s_actions		*_actions;
  bool	_admin;
  bool	_started;
};


#endif
