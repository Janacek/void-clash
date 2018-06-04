//
// NetworkEngine.cpp for pfa in /home/volent/dev/pfa/client/NetworkEngine
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Thu Sep 19 15:38:20 2013 florent espanet
// Last update Fri Dec 27 20:44:43 2013 Gegon
//

#include	"NetworkEngine.hpp"
#include	"Manager.hpp"
#include	"Sender.hpp"
#include        "Bullet.hpp"
#include        "Bomb.hpp"
#include        "Explosion.hpp"
#include        "GravityField.hpp"
#include        "SpeedField.hpp"
#include	"main.hpp"
#include	"Map.hpp"

//////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
//////////////////////////////////////////////////////////////////////

NetworkEngine::NetworkEngine()
{
  _receiver = new Receiver(this);
  _sender = new Sender(this);
}

NetworkEngine::~NetworkEngine(void)
{
}

//////////////////////////////////////////////////////////////////////
/////	IEngine function
//////////////////////////////////////////////////////////////////////

void	NetworkEngine::start(void)
{
  _socket.setBlocking(false);

  //sf::Time timeout = sf::seconds(0.5f);
  //_publicIp = sf::IpAddress::getPublicAddress(timeout).toInteger();
  //_localIp = sf::IpAddress::getLocalAddress().toInteger();
}

//------------------------------------------------------------------//

void	NetworkEngine::stop(void)
{
  delete _receiver;
  delete _sender;
}

//------------------------------------------------------------------//

eGameState	NetworkEngine::update(const sf::Time &)
{
  return RUN;
}

//////////////////////////////////////////////////////////////////////
/////	Tools
//////////////////////////////////////////////////////////////////////

void		NetworkEngine::printLog(int level, const std::string &msg, const char *color)
{
  if (level > LOG_LEVEL)
    return ;

  if (color)
    std::cout << color;
  else
    {
      if (level == 1)
	std::cout << RED;
      if (level == 2)
	std::cout << YELLOW;
      if (level == 3)
	std::cout << GREEN;
    }
  std::cout << S_Map->getClock().getElapsedTime().asMilliseconds() << " - " << msg << RESET << std::endl;
}

void		NetworkEngine::printLogWithId(int level, const std::string &msg, const sf::Uint32 &id, const char *color)
{
  if (level > LOG_LEVEL)
    return ;

  if (color)
    std::cout << color;
  else
    {
      if (level == 1)
	std::cout << RED;
      if (level == 2)
	std::cout << YELLOW;
      if (level == 3)
	std::cout << GREEN;
    }
  std::cout << S_Map->getClock().getElapsedTime().asMilliseconds() << " - " << msg << id << RESET << std::endl;
}

//////////////////////////////////////////////////////////////////////
/////	Getters
//////////////////////////////////////////////////////////////////////

sf::UdpSocket	&NetworkEngine::getSocket(void)
{
  return (_socket);
}

//------------------------------------------------------------------//

Receiver	*NetworkEngine::getReceiver(void)
{
  return (_receiver);
}

//------------------------------------------------------------------//

Sender		*NetworkEngine::getSender(void)
{
  return (_sender);
}

//------------------------------------------------------------------//

std::string	&NetworkEngine::getServerIp(void)
{
  return (_serverIp);
}

//------------------------------------------------------------------//

unsigned short	NetworkEngine::getServerPort(void)
{
  return (_serverPort);
}

//////////////////////////////////////////////////////////////////////
/////	Setters
//////////////////////////////////////////////////////////////////////

void	NetworkEngine::setHost(const char *host)
{
  _serverIp = host;
}

//------------------------------------------------------------------//

void	NetworkEngine::setPort(const int &port)
{
  _serverPort = port;
}
