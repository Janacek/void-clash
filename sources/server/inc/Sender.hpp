//
// Sender.hpp for Void Clash in /home/gegon/pfa/server
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Wed Sep 25 17:22:52 2013 Gegon
// Last update Thu Jan 30 22:34:06 2014 Gegon
//

#ifndef		SENDER_HPP_
# define	SENDER_HPP_

#include	"AObject.hpp"

class	NetworkEngine;

///////////////////////////////////////////////
/////   Defines the packet type 
/////	that will be sent to clients

class	Sender
{
public:
  Sender(NetworkEngine	*);
  ~Sender();

  void	update();

  // Send packet
  void	sendPacket(sf::Packet &packet, ClientHandle *client = NULL);

  // Generic packet send
  void	sendPacketGeneric(ePacketType packetType, ClientHandle *client = NULL);

  void	insertAddedObj();
  void	insertDeletedObj();

  void	insertWorldData();
  template<typename T>
  void	insertObject(AObject *obj);
  template<typename T>
  void	insertWeapons(AObject *obj);

  // EVENTS
  void	sendUpdateEvents();
  // Specific packet about events on 1 object
  void	sendPacketEvent(ePacketType packetType, sf::Uint32 id, ClientHandle *client = NULL);
  void	sendPacketEvent(ePacketType packetType, sf::Uint32 id, sf::IpAddress ip, unsigned short port);
  // Specific packet about events on 2 id (ex kills)
  void	sendPacketEventTwo(ePacketType packetType, sf::Uint32 id, sf::Uint32 id2, ClientHandle *client = NULL);
  void	sendStringToDisplay(std::string str, int level, ClientHandle *client = NULL);
  // Pong
  void	sendPing(float timestamp, ClientHandle *client);
  // Config
  void	sendConfig(ClientHandle *client = NULL);
  // Weapon selection
  void	sendWeaponSelection(ClientHandle *client = NULL);
  // Check client activity to send deco
  void	checkClientActivity();
  // Map list
  void	sendMapList(ClientHandle *client);

private:
  void	createPacket(ePacketType packetType);
  void	sendPacketTo(ClientHandle *client = NULL);
  void	sendPacketTo(sf::IpAddress ip, unsigned short port);

  sf::Packet	_packet;
  ePacketType	_packetType;

  NetworkEngine	*_networkEngine;
};

#endif
