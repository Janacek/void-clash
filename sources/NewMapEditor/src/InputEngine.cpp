//
// InputEngine.cpp for Void Clash in /home/gegon/pfa/client/InputEngine/inc
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Tue Sep 10 14:38:07 2013 Gegon
// Last update Mon Feb 24 16:07:52 2014 Gegon
//

#include <cstring>
#include <cmath>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <SFML/Audio.hpp>
#include <Context.hpp>
#include "InputEngine.hpp"
#include "Manager.hpp"

#include "Defines.h"
#include "ConfigParser.hpp"
#include "HudRessources.hpp"

// Globals
extern t_config *G_conf;
extern int sizeX;
extern int sizeY;
extern bool G_isMapEditor;

//////////////////////////////////////////////////////////////////////
/////	Ctor / Dtor
//////////////////////////////////////////////////////////////////////

InputEngine::InputEngine()
{
	_focus = true;
	_chat = false;
	_gameState = RUN;
}

InputEngine::~InputEngine(void)
{
}

//////////////////////////////////////////////////////////////////////
/////	IEngine function
//////////////////////////////////////////////////////////////////////

void	InputEngine::start(void)
{
	_playerActions = new s_actions;
	memset(_playerActions, 0, sizeof(*_playerActions));
	_msg = new sf::String;
	_msg->clear();
	_lastMsg = new sf::String;
	_oldMousePosX = sf::Mouse::getPosition().x;
	_oldMousePosY = sf::Mouse::getPosition().y;
	_distanceX = 0;
	_distanceY = 0;
	_rightClickPressed = false;
	_leftClickPressed = false;
}

//------------------------------------------------------------------//

void	InputEngine::stop(void)
{
	delete _playerActions;
	delete _msg;
	delete _lastMsg;
}

//------------------------------------------------------------------//

eGameState	InputEngine::update(const sf::Time &)
{
	// Player actions reset and event
	resetActions();
	Event::addEvent(ev_PLAYER_ACTION, s_event(S_Map->getCurrentPlayer(), _playerActions));

	bool inGui = false;

	float newMousePosX = sf::Mouse::getPosition().x;
	float newMousePosY = sf::Mouse::getPosition().y;

	_distanceX = _oldMousePosX - newMousePosX;
	_distanceY = _oldMousePosY - newMousePosY;

	_oldMousePosX = newMousePosX;
	_oldMousePosY = newMousePosY;

	if (_leftClickPressed)
	{
		if (abs(_leftClickPositionX - sf::Mouse::getPosition().x) > 1 ||
			abs(_leftClickPositionY - sf::Mouse::getPosition().y) > 1)
		{
			std::pair<float, float> *positions = new std::pair<float, float>(_leftClickPositionX, _leftClickPositionY);

			Event::addEvent(ev_OBJECT_CREATION, s_event(NULL, positions));
		}
	}

	if (_rightClickPressed)
	{
		std::shared_ptr<Player>	player = S_Map->getPlayerFollowed();

		player->setPosition(player->getPosition().first + _distanceX * 3, player->getPosition().second + _distanceY * 3);
		_distanceX = 0;
		_distanceY = 0;
	}

		// check if focus is off or game exit
		_gameState = checkEvents();

		if (!_focus || (_gameState != RUN && _gameState != MENU))
			return (_gameState);


		// Do not take input if player is respawning
		if (S_Map->getCurrentPlayer() && S_Map->getCurrentPlayer()->isRespawning())
			return (_gameState);

		checkKeysPressed();
		checkPlayerActions();

		
	return (_gameState);
}

//////////////////////////////////////////////////////////////////////
/////	Key pressed check
/////	Used to check all inputs except player actions
/////	Send events
//////////////////////////////////////////////////////////////////////

void	InputEngine::checkKeysPressed()
{
	// Reload config
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		S_Map->loadConfig("config.json");

	// Check tab
	HudRessources::getInstance()->setTab(false);
	if (_gameState != MENU && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
		HudRessources::getInstance()->setTab(true);

	checkJoinTeam();
	checkSwapTeam();
}

//------------------------------------------------------------------//

void	InputEngine::checkJoinTeam(void)
{
	HudRessources::getInstance()->setTeamJoined(0);
	if (_chat == false && _focus == true
		&& (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)))
		HudRessources::getInstance()->setTeamJoined(1);
	else if (_chat == false && _focus == true
		&& (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)))
		HudRessources::getInstance()->setTeamJoined(2);
}

//------------------------------------------------------------------//

void		InputEngine::checkSwapTeam(void)
{
	HudRessources::getInstance()->setTeamSwap(false);
	if (_chat == false && _focus == true && sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		HudRessources::getInstance()->setTeamSwap(true);
}
//////////////////////////////////////////////////////////////////////
/////	Player actions
//////////////////////////////////////////////////////////////////////

void		InputEngine::resetActions(void)
{
	float saveX = _playerActions->aimX;
	float saveY = _playerActions->aimY;
	memset(_playerActions, 0, sizeof(*_playerActions));
	_playerActions->aimX = saveX;
	_playerActions->aimY = saveY;
}

//------------------------------------------------------------------//

void		InputEngine::checkPlayerActions(void)
{
		checkMove();
		checkAim();
		checkLeftClick();
		checkRightClick();
		checkSwitchPrimary();
		checkSwitchSecondary();
}

//------------------------------------------------------------------//

void		InputEngine::checkLeftClick(void)
{
	if (checkActionSec(FIRE_PRIMARY))
		_playerActions->primary = true;
}

//------------------------------------------------------------------//

void		InputEngine::checkRightClick(void)
{
	if (_gameState != MENU)
		if (checkActionSec(FIRE_SECONDARY))
			_playerActions->secondary = true;
}

//------------------------------------------------------------------//

void		InputEngine::checkSwitchPrimary(void)
{
	if (checkActionSec(SWITCH_PRIMARY))
	{
		if (!_filterSwitchPrimary)
		{
//			_playerActions->switchPrimary = true;
		}
		_filterSwitchPrimary = true;
	}
	else
		_filterSwitchPrimary = false;
}

//------------------------------------------------------------------//

void		InputEngine::checkSwitchSecondary(void)
{
	if (checkActionSec(SWITCH_SECONDARY))
	{
		if (!_filterSwitchSecondary)
//			_playerActions->switchSecondary = true;
		_filterSwitchSecondary = true;
	}
	else
		_filterSwitchSecondary = false;
}


//------------------------------------------------------------------//

void		InputEngine::checkMove(void)
{
	if (sf::Joystick::isConnected(1) == true)
	{
		// if (sf::Joystick::getAxisPosition(1, sf::Joystick::X) > 0)
		// 	_playerActions->moveX = 100;
		// if (sf::Joystick::getAxisPosition(1, sf::Joystick::X) < 0)
		// 	_playerActions->moveX = -100;
		// if (sf::Joystick::getAxisPosition(1, sf::Joystick::Y) > 0)
		// 	_playerActions->moveY = 100;
		// if (sf::Joystick::getAxisPosition(1, sf::Joystick::Y) < 0)
		// 	_playerActions->moveY = -100;
		_playerActions->moveX = (sf::Joystick::getAxisPosition(1, sf::Joystick::X));
		_playerActions->moveY = (sf::Joystick::getAxisPosition(1, sf::Joystick::Y));
		// std::cout << _playerActions->moveX << std::endl;
	}
	else
	{
		if (checkActionSec(MOVE_LEFT))
			_playerActions->moveX -= 100;
		if (checkActionSec(MOVE_RIGHT))
			_playerActions->moveX += 100;
		if (checkActionSec(MOVE_UP))
			_playerActions->moveY -= 100;
		if (checkActionSec(MOVE_DOWN))
			_playerActions->moveY += 100;
	}
}

//------------------------------------------------------------------//

void		InputEngine::checkAim(void)
{
	float	aimX;
	float	aimY;

  if (sf::Joystick::isConnected(1) == true)
    {
      aimX = sf::Joystick::getAxisPosition(1, sf::Joystick::Z);
      aimY = sf::Joystick::getAxisPosition(1, sf::Joystick::R);
      if (std::abs(aimX) < 80 && std::abs(aimY) < 80)
      	return ;
    }
  else
    {
	    const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());
	    screenToWorld(mouse.x, mouse.y, aimX, aimY);
    }

	_playerActions->aimX = aimX;
	_playerActions->aimY = aimY;
}

//////////////////////////////////////////////////////////////////////
/////	Check SFML main events
//////////////////////////////////////////////////////////////////////
eGameState	InputEngine::checkEvents(void)
{
	sf::RenderWindow *window = &S_Context->getWindow();
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

	while (window->pollEvent(_event))
	{
		if (_event.type == sf::Event::TextEntered)
		{
			//if ((_event.text.unicode >= '0' && _event.text.unicode <= '9') ||
			//	_event.text.unicode == '.')
				context.injectChar(_event.text.unicode);
		}

		if (_event.type == sf::Event::KeyPressed)
		{
			if (_event.key.code == sf::Keyboard::BackSpace)
			{
				context.injectKeyDown(CEGUI::Key::Backspace);
			}
			else if (_event.key.code == sf::Keyboard::Left)
			{
				context.injectKeyDown(CEGUI::Key::ArrowLeft);
			}
			else if (_event.key.code == sf::Keyboard::Right)
			{
				context.injectKeyDown(CEGUI::Key::ArrowRight);
			}
			else if (_event.key.code == sf::Keyboard::Up)
			{
				context.injectKeyDown(CEGUI::Key::ArrowUp);
			}
			else if (_event.key.code == sf::Keyboard::Down)
			{
				context.injectKeyDown(CEGUI::Key::ArrowDown);
			}
		}

		if (_event.type == sf::Event::KeyReleased)
		{
			if (_event.key.code == sf::Keyboard::BackSpace)
			{
				context.injectKeyUp(CEGUI::Key::Backspace);
			}
			else if (_event.key.code == sf::Keyboard::Left)
			{
				context.injectKeyUp(CEGUI::Key::ArrowLeft);
			}
			else if (_event.key.code == sf::Keyboard::Right)
			{
				context.injectKeyUp(CEGUI::Key::ArrowRight);
			}
			else if (_event.key.code == sf::Keyboard::Up)
			{
				context.injectKeyUp(CEGUI::Key::ArrowUp);
			}
			else if (_event.key.code == sf::Keyboard::Down)
			{
				context.injectKeyUp(CEGUI::Key::ArrowDown);
			}
		}

		if (_event.type == sf::Event::MouseWheelMoved)
		{
			bool foc = context.injectMouseWheelChange(_event.mouseWheel.delta);

			if (!foc)
			{
				static float initedZoom;
				if (!G_isMapEditor)
					initedZoom = G_conf->game->zoom;
				else
					initedZoom = 10;

				float zoom = S_Map->getZoom();
				if (zoom < 0)
					zoom = initedZoom;
				S_Map->setZoom(std::min(std::max(0.1f, zoom - (float)_event.mouseWheel.delta / 10), initedZoom));
			}
		}
		if (_event.type == sf::Event::LostFocus)
		{
			_focus = false;
			return (RUN);
		}
		if (_event.type == sf::Event::GainedFocus)
		{
			_focus = true;
			return (RUN);
		}
		if (_event.type == sf::Event::Closed)
			return (EXIT);
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::F1)
			return EXIT;
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::F3)
			Event::addEvent(ev_REBOOT_GRAPHIC, s_event());
		if (checkAction(MUTE_SOUND))
			Event::addEvent(ev_MUTE_SOUND, s_event());

		if (_event.type == sf::Event::MouseMoved)
		{
			context.injectMousePosition((float)(sf::Mouse::getPosition(*window).x), (float)(sf::Mouse::getPosition(*window).y));
		}


		if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Right)
		{
			_rightClickPressed = true;
		}
		if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Right)
		{
			_rightClickPressed = false;
		}

		if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left)
		{
			_leftClickPressed = true;
			bool injected = context.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);

			if (injected == true)
			{
				_leftClickPressed = false;
			}

			_leftClickPositionX = sf::Mouse::getPosition().x;
			_leftClickPositionY = sf::Mouse::getPosition().y;
		}

		if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left)
		{
			_leftClickPressed = false;
			bool injected = context.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);

			if (injected == false)
			{

				if (abs(_leftClickPositionX - sf::Mouse::getPosition().x) <= 1 &&
					abs(_leftClickPositionY - sf::Mouse::getPosition().y) <= 1)
				{

					const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());
					float aimX = 0; float aimY = 0;
					screenToWorld(mouse.x, mouse.y, aimX, aimY);
					std::shared_ptr<AObject>obj = S_Map->findObjectAtPosition(aimX, aimY);


					if (obj)
					{
						Event::addEvent(ev_OBJECT_SELECTED, obj);
					}
					else
					{
						Event::addEvent(ev_OBJECT_DESELECTED);
					}
				}
				else if (abs(_leftClickPositionX - sf::Mouse::getPosition().x) > 1 ||
					abs(_leftClickPositionY - sf::Mouse::getPosition().y) > 1)
				{
					Event::addEvent(ev_OBJECT_CREATED);
				}
			}
		}
	}



	if (_event.type == sf::Event::Closed)
		return (EXIT);
	if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::F1)
		return EXIT;


	return (_gameState);
}

//////////////////////////////////////////////////////////////////////
/////	Chat management
//////////////////////////////////////////////////////////////////////

void	InputEngine::handleChatInputKey(void)
{
	// Return
	if (_event.key.code == '\b')
	{
		Event::addEvent(ev_CHAT_MSG_CHANGE, s_event(NULL, _msg));
		if (_msg->isEmpty() == false)
			_msg->erase(_msg->getSize() - 1, 1);
	}
	// Echap
	else if (_event.key.code == 27)
	{
		Event::addEvent(ev_CHAT_SWITCH, s_event());
		_msg->clear();
		_chat = false;
		HudRessources::getInstance()->setChatString("");
	}
	else
	{
		sf::String input = _event.text.unicode;
		*_msg += input;
		Event::addEvent(ev_CHAT_MSG_CHANGE, s_event(NULL, _msg));
	}
}

//////////////////////////////////////////////////////////////////////
/////	Getters
//////////////////////////////////////////////////////////////////////

bool		InputEngine::chatEnabled(void) const
{
	return _chat;
}

//------------------------------------------------------------------//

sf::String	*InputEngine::getMessage(void)
{
	return _msg;
}

//////////////////////////////////////////////////////////////////////
/////	Setters
//////////////////////////////////////////////////////////////////////

void		InputEngine::setGameState(eGameState gameState)
{
	_gameState = gameState;
}


//////////////////////////////////////////////////////////////////////
/////	CEGUI
//////////////////////////////////////////////////////////////////////

eGameState		InputEngine::injectInputsIntoCEGUIEngines(sf::Event &event)
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	sf::RenderWindow &window = S_Context->getWindow();

	while (window.pollEvent(_event))
	{
		if (event.type == sf::Event::MouseMoved)
		{
			context.injectMousePosition((float)(sf::Mouse::getPosition(window).x), (float)(sf::Mouse::getPosition(window).y));
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			context.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			context.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
		}
		if (event.type == sf::Event::MouseWheelMoved)
		{
			context.injectMouseWheelChange(event.mouseWheel.delta);
		}

		if (_event.type == sf::Event::Closed)
			return (EXIT);
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::F1)
			return EXIT;
	}
	return RUN;
}

bool InputEngine::checkAction(UserActions action)
{
	if ((_event.type == sf::Event::KeyPressed &&
		_event.key.code == _userKeys._keyConf[action].first) ||
		(_event.type == sf::Event::MouseButtonPressed &&
		_event.mouseButton.button == _userKeys._keyConf[action].second))
		return true;
	return false;
}

bool InputEngine::checkActionSec(UserActions action)
{
	if ((sf::Keyboard::isKeyPressed(_userKeys._keyConf[action].first)) ||
		(sf::Mouse::isButtonPressed(_userKeys._keyConf[action].second)))
		return true;
	return false;
}
