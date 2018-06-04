//
// Receiver.hpp for Void Clash in /home/gegon/pfa/server/NetworkEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Wed Oct  9 14:43:29 2013 Gegon
// Last update Fri Dec 20 00:13:27 2013 Gegon
//

#ifndef		RECEIVER_HPP_
# define	RECEIVER_HPP_

#include	"AObject.hpp"

class	NetworkEngine;

///////////////////////////////////////////////
/////   Defines the packet type 
/////	that will be sent to clients

class	Command;

class	Receiver
{
public:
  Receiver(NetworkEngine *);
  ~Receiver();

  bool update();

private:
	void	updateClientInput(sf::IpAddress ip, unsigned short port);
	void	handleChat(sf::IpAddress ip, unsigned short port);
	void	playerJoinTeam(sf::IpAddress ip, unsigned short port);
	void	handleWeaponSelection(sf::IpAddress ip, unsigned short port);
	void	handleChangeNick(sf::IpAddress ip, unsigned short port);

	void	handleQuit(sf::IpAddress ip, unsigned short port);
	void	handleRequestSwitchMap(sf::IpAddress ip, unsigned short port);
	void	handleRequestSwitchMode(sf::IpAddress ip, unsigned short port);
	void	handleRequestPlayerKick(sf::IpAddress ip, unsigned short port);


	void	updateClientActivity(sf::IpAddress ip, unsigned short port);
	void	checkClientsActivity();


  sf::Packet	_packet;

  Command	*_command;

  NetworkEngine	*_networkEngine;
};

#endif
