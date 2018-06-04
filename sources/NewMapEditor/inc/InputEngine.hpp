//
// InputEngine.hpp for Void Clash in /home/gegon/pfa/client/InputEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Tue Sep 10 14:35:37 2013 Gegon
// Last update Wed Feb 12 05:16:09 2014 sadin_n
//

#ifndef         __INPUTENGINE_HPP__
# define        __INPUTENGINE_HPP__
#include	<CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include	<CEGUI/CEGUI.h>

#include	"GUIManager.h"
#include	<SFML/Window/Event.hpp>
#include	"AEngine.hpp"
#include	"Actions.hpp"


///////////////////////////////////////////////
/////   Data to send


///////////////////////////////////////////////
/////   This class is used to parse player inputs

////	TODO - LINK WITH USER KEYBINDS

#include "KeysParameters.h"

class   InputEngine : public AEngine
{
public :
  InputEngine();
  virtual ~InputEngine(void);

  virtual void start(void);
  virtual void stop(void);
  virtual eGameState update(const sf::Time &deltaTime);

  void checkKeysPressed();
  void handleChatInputKey();
  eGameState checkEvents();

  // Team management
  void checkJoinTeam();
  void checkSwapTeam();

  // <CEGUI>
  eGameState injectInputsIntoCEGUIEngines(sf::Event &);
  // </CEGUI>

  bool checkAction(UserActions);
  bool checkActionSec(UserActions);

  // Player actions
  void checkAim(void);
  void checkRightClick(void);
  void checkMove(void);
  void checkLeftClick(void);
  void checkPlayerActions(void);
  void resetActions(void);
  void		checkSwitchPrimary(void);
  void		checkSwitchSecondary(void);

  // Getters
  bool chatEnabled() const;
  sf::String *getMessage();
  
  // Setters
  void	setGameState(eGameState);
private :
  eGameState	_gameState;
  bool		_focus;
  bool		_chat;
  s_actions	*_playerActions;
  sf::Event	_event;
  sf::String	*_msg;
  sf::String	*_lastMsg;

  bool		_filterSwitchPrimary;
  bool		_filterSwitchSecondary;


  float				_oldMousePosX;
  float				_oldMousePosY;

  float				_distanceX;
  float				_distanceY;

  bool				_rightClickPressed;
  bool				_leftClickPressed;

  float				_leftClickPositionX;
  float				_leftClickPositionY;

  KeyParameters		_userKeys;
};

#endif		// __INPUTENGINE_HPP__
