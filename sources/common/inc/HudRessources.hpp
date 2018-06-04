//
// HudRessources.hpp for Void Clash in /home/gegon/pfa/client/GameEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Mon Oct  7 23:15:41 2013 Gegon
// Last update Fri Jan 31 21:42:44 2014 Gegon
//

#ifndef		__HUDRESSOURCES_H__
# define	__HUDRESSOURCES_H__

#include	"Player.hpp"
#include	<list>
#include	<SFML/System.hpp>

///////////////////////////////////////////////
/////   Message handler structure used for chat

typedef	struct	s_msgHandler
{
  sf::Uint32 _id;
  sf::String _msg;
  bool _isNew;
  int _level; // Used for server messages, 1 succes 0 error
  // TIMESTAMP
}		t_msgHandler;

///////////////////////////////////////////////
/////   Kills handler 

typedef struct	s_killHandler
{
	std::shared_ptr<Player>	_killer;
  Player *	_killed;
  eObjectType	_hitterType;
  bool		_isNew;
  // TIMESTAMP
}		t_killHandler;

class		HudRessources
{
public:
	static HudRessources* getInstance(void);

  void	setTab(bool value);

  std::list<t_killHandler *>	&getKillsReport();

  // Chat
  void setChat(bool value);
  std::list<t_msgHandler *>	&getChat();
  bool	isTabActive() const;
  bool isChatActive(void);
  void	setChatString(std::string msg);
  const std::string	&getChatString();

  // Team
  void	setTeamJoined(sf::Int16 teamId);
  void	setTeamSwap(bool swap);
  sf::Int16	getTeamJoined();
  bool	getTeamSwap();

  // Mouse position
  void setMousePosition(const sf::Vector2i &mousePosition);
  const sf::Vector2i& getMousePosition(void);

private:
  HudRessources(void);
  virtual ~HudRessources(void);

  static HudRessources		*_instance;

  std::list<t_msgHandler *>	_chat;
  std::list<t_killHandler *>	_killsReport;
  bool				_tab;

  bool				_chatActive;
  std::string		_chatString;

  // Team related
  sf::Int16		_teamJoined;	// 0 if nothing
  bool			_teamSwap;

  // Save mouse position
  sf::Vector2i _mousePosition;
};

#endif		/* !__HUDRESSOURCES_H__ */
