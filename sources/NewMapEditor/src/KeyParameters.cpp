#include "KeysParameters.h"

KeyParameters::KeyParameters()
{
	loadKeys();
}

KeyParameters::~KeyParameters()
{

}

void KeyParameters::setKey(UserActions action, sf::Keyboard::Key key, sf::Mouse::Button button)
{
	_keyConf[action] = std::pair<sf::Keyboard::Key, sf::Mouse::Button>(key, button);
}

void KeyParameters::loadKeys()
{
	t_keyPrefs *keys;
	KeyPreferencesParser *parser = new KeyPreferencesParser();
	
	if (!parser->loadFile("keysPreferences.json"))
	{
		std::cerr << "Error while loading \"keysPreferences.json\" file" << std::endl;
		return;
	}

	keys = (t_keyPrefs *)parser->parse();

	setKey(MOVE_LEFT, keys->moveLeft.first, keys->moveLeft.second);
	setKey(MOVE_RIGHT, keys->moveRight.first, keys->moveRight.second);
	setKey(MOVE_UP, keys->moveUp.first, keys->moveUp.second);
	setKey(MOVE_DOWN, keys->moveDown.first, keys->moveDown.second);
	setKey(FIRE_PRIMARY, keys->firePrimary.first, keys->firePrimary.second);
	setKey(FIRE_SECONDARY, keys->fireSecondary.first, keys->fireSecondary.second);
	setKey(SWITCH_PRIMARY, keys->switchPrimary.first, keys->switchPrimary.second);
	setKey(SWITCH_SECONDARY, keys->switchSecondary.first, keys->switchSecondary.second);
	setKey(WEAPON_SELECTION_SHORTCUT, keys->weaponsSelectionShortcut.first, keys->weaponsSelectionShortcut.second);
	setKey(MUTE_SOUND, keys->muteSound.first, keys->muteSound.second);

	userActionsToString[MOVE_LEFT] = "Move Left";
	userActionsToString[MOVE_RIGHT] = "Move Right";
	userActionsToString[MOVE_UP] = "Move Up";
	userActionsToString[MOVE_DOWN] = "Move Down";
	userActionsToString[FIRE_PRIMARY] = "Fire primary";
	userActionsToString[FIRE_SECONDARY] = "Fire secondary";
	userActionsToString[SWITCH_PRIMARY] = "Switch primary";
	userActionsToString[SWITCH_SECONDARY] = "Switch secondary";
	userActionsToString[WEAPON_SELECTION_SHORTCUT] = "Weapon selection shortcut";
	userActionsToString[MUTE_SOUND] = "Mute Sound";

	delete parser;

	lastBindings.first = (sf::Keyboard::Key) - 1;
	lastBindings.second = (sf::Mouse::Button) - 1;

	newKeySelection = false;
}