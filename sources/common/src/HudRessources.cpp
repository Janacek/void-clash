//
// HudRessources.cpp for Void Clash in /home/gegon/pfa/client/GameEngine/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Mon Oct  7 23:14:57 2013 Gegon
// Last update Fri Oct 11 01:17:25 2013 Gegon
//

#include	<cstdlib>
#include	"HudRessources.hpp"

HudRessources     *HudRessources::_instance = NULL;

HudRessources::HudRessources(void)
{
	_chatActive = false;
	_tab = false;
	_teamJoined = 0;
	_teamSwap = false;
}

HudRessources::~HudRessources(void)
{
}

///////////////////////////////////////////////
/////   Setterz

void	HudRessources::setTab(bool value)
{
  _tab = value;
}

void HudRessources::setChat(bool value)
{
	_chatActive = value;
}

///////////////////////////////////////////////
/////   Getterz

HudRessources*    HudRessources::getInstance(void)
{
  if (HudRessources::_instance == NULL)
    HudRessources::_instance = new HudRessources;
  return HudRessources::_instance;
}

std::list<t_msgHandler *>	&HudRessources::getChat()
{
  return (_chat);
}

std::list<t_killHandler *>	&HudRessources::getKillsReport()
{
  return (_killsReport);
}

bool HudRessources::isChatActive(void)
{
	return _chatActive;
}

bool	HudRessources::isTabActive() const
{
  return (_tab);
}

void	HudRessources::setTeamJoined(sf::Int16 teamId)
{
	_teamJoined = teamId;
}

void	HudRessources::setTeamSwap(bool swap)
{
	_teamSwap = swap;
}

sf::Int16	HudRessources::getTeamJoined()
{
	return _teamJoined;
}

bool	HudRessources::getTeamSwap()
{
	return _teamSwap;
}

void	HudRessources::setChatString(std::string msg)
{
	_chatString = msg;
}

const std::string	&HudRessources::getChatString()
{
	return _chatString;
}

void HudRessources::setMousePosition(const sf::Vector2i &mousePosition)
{
	_mousePosition = mousePosition;
}

const sf::Vector2i& HudRessources::getMousePosition(void)
{
	return _mousePosition;
}