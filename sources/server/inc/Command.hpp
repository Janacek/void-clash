//
// Command.hpp for pfa in /home/volent/dev/newpfa
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Fri Nov 22 12:56:47 2013 florent espanet
// Last update Fri Jan 17 03:27:29 2014 Gegon
//

#ifndef COMMAND_HPP_
# define COMMAND_HPP_

#include <iostream>
#include "Manager.hpp"
#include "NetworkEngine.hpp"
#include "Map.hpp"

class Command
{
public:
  Command(NetworkEngine *networkEngine);
  ~Command();

  bool chatHandle(const std::string &, sf::IpAddress ip, unsigned short port);

private:
  void reloadConfig();
  void nextMap();
  void prevMap();
  void unknown(sf::IpAddress ip, unsigned short port);
  void authRequest(const std::string &password, sf::IpAddress ip, unsigned short port);
  void changeNick(const std::string &nick, sf::IpAddress ip, unsigned short port);
  void kick(sf::Uint32 id);
  void slap(sf::Uint32 id);
  void help();
  void switchMode(enum eMapMode mode);

private:
  NetworkEngine *_networkEngine;
};

#endif /* COMMAND_HPP_ */
