#ifndef KEYSPARAMETERS_H_
#define KEYSPARAMETERS_H_

#include <map>

#include	<SFML/Window/Event.hpp>
#include "KeyPreferencesParser.h"

enum UserActions
{
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_UP,
	FIRE_PRIMARY,
	FIRE_SECONDARY,
	SWITCH_PRIMARY,
	SWITCH_SECONDARY,
	WEAPON_SELECTION_SHORTCUT,
	MUTE_SOUND,
	COUNT = 10,
};

class KeyParameters
{
public:
	KeyParameters();
	~KeyParameters();

	void loadKeys();
	void setKey(UserActions, sf::Keyboard::Key, sf::Mouse::Button);
	std::map<UserActions, std::pair<sf::Keyboard::Key, sf::Mouse::Button>>	_keyConf;

	std::map<UserActions, std::string> userActionsToString;

	std::pair<sf::Keyboard::Key, sf::Mouse::Button>	lastBindings;
	bool newKeySelection;
private:
};

#endif