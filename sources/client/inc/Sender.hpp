//
// Sender.hpp for Void Clash in /home/gegon/pfa/client/NetworkEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Sat Oct  5 00:01:09 2013 Gegon
// Last update Fri Dec 27 17:03:23 2013 Gegon
//

#ifndef		SENDER_HPP_
# define	SENDER_HPP_

#include	<SFML/Network.hpp>
#include	"NetworkEngine.hpp"

class		Sender
{
public:
  Sender(NetworkEngine *);
  ~Sender(void);

  bool  update(void);
  void	sendPong(float timestamp);
  void	sendConfirmation(enum ePacketType receivedPacket);
  void	sendEvents();
  void	finishReliableUdpSequence();

private:
  // FUNCS
	void	checkReliableUDP();
	void	sendQuit();
  void	sendInput(void);
  void	sendMessage(void);
  void	sendConfig(void);
  void	sendMapChange(void);
  void	sendModeChange(void);
  void	sendPlayerKick(void);
  void	sendHordeActivation(void);
  void	sendSwitchTeam();
  void	sendChangeNick();

  // VARS
  s_actions	*_playerActions; // Only use for packet transmission, only Sender use it
  sf::Packet	_packet;
  NetworkEngine	*_networkEngine;

  // Reliable UDP
  sf::Clock		_packetClock;
  ePacketType	_packetSequence;
};

#endif
