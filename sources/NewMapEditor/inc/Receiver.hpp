//
// Receiver.hpp for Void Clash in /home/gegon/pfa/client/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Fri Oct  4 22:57:28 2013 Gegon
// Last update Fri Jan 31 17:29:27 2014 Gegon
//

#ifndef		RECEIVER_HPP_
# define	RECEIVER_HPP_

class	NetworkEngine;

class	Receiver
{
public:
  Receiver(NetworkEngine *);
  ~Receiver();

  bool	update();

private:
  // FUNCS
  bool	extractPacketFromType(sf::Uint32 packetType);

  void	extractWorldData();
  bool	extractObject(sf::Uint32 type, sf::Uint32 id);
  void	updatePlayer(sf::Uint32 id);
  void	registerNewPlayer(sf::Uint32 id, sf::Uint16 team);

  template<typename T>
  void	extractObject(sf::Uint32 id);
  template<typename T>
  T	*extractWeapons(sf::Uint32 id);

  void	handleUpdate();
  void	handleWelcome();
  void	handleSynchro();
  void	handleConfig();
  void	handlePing();

  void	handleWeaponSelection();
  void	handleKillEvent();
  void	handlePlayerHitEvent();
  void	handleChatEvent();
  void	handleDeleteEvent();
  void	handleLaunchEvent();
  void	handleIdentification();
  void	handleSwitchMapMode();
  void	handleCaptureFlag();
  void	handleKick();
  void	handleTurretDestroyed();
  void	handleMapList();
  void	handleResetRound();

  void	createServerConnectivityEvent();

  // VARS
  sf::Packet	_packet;

  // Used for packet udpate ordering
  float		_lastUpdate;

  // Used to check no connectivity with serv
  sf::Time	_lastConnectivity;
  unsigned int	_msSinceLastConnectivity;

  NetworkEngine	*_networkEngine;
};

#endif
