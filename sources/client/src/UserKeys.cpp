/********************************************************************
**
**		UserKeys.cpp
**		Created by : Vial Joris
**
********************************************************************/

#include "UserKeys.hpp"
#include "Log.hpp"
#include "VoidClash_OpenGL.h"
#include "AssetPath.h"

/////////////////////////////////////////////////////////////////////
/////	Ctor / Dtor
/////////////////////////////////////////////////////////////////////

CUserKeys::CUserKeys(void)
{
	_active = false;
	_customKey = GLFW_KEY_UNKNOWN;
}

CUserKeys::~CUserKeys(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Set the config file path
/////////////////////////////////////////////////////////////////////

void CUserKeys::setConfigFilePath(const std::string &path)
{
	_path = path;
}

/////////////////////////////////////////////////////////////////////
/////	Load the config file
/////////////////////////////////////////////////////////////////////

void CUserKeys::loadConfig(void)
{
	VC_INFO_CRITICAL("CUserKeys::Loading key configuration from " + _path);

	// Generate GLFW string
	generateGLFWStrings();

	// Load default config first
	loadDefaultConfig();

	TiXmlDocument file(_path.c_str());

	// Try to load config file, if it failed, set default values
	if (!file.LoadFile())
		return fail();

	// Load config file
	TiXmlElement *e = file.FirstChildElement();
	if (e == NULL) return fail();

	if (!loadKey("FIRE_PRIMARY", e)) fail();
	if (!loadKey("FIRE_SECONDARY", e)) fail();
	if (!loadKey("MOVE_UP", e)) fail();
	if (!loadKey("MOVE_DOWN", e)) fail();
	if (!loadKey("MOVE_LEFT", e)) fail();
	if (!loadKey("MOVE_RIGHT", e)) fail();
}

/////////////////////////////////////////////////////////////////////
/////	Load a key
/////////////////////////////////////////////////////////////////////

bool CUserKeys::loadKey(const std::string &name, TiXmlElement *e)
{
	// Get action to change
	UserAction action = stringToAction(name);
	if (action == ACTION_MAX)
		return false;

	// Get value to apply
	int value = std::atoi(e->Attribute(name.c_str()));

	// Update binding
	setNewBind(action, value);

	VC_INFO_CRITICAL("CUserKeys::loadKey, " + name + ": " + std::to_string(value));

	return true;
}

/////////////////////////////////////////////////////////////////////
/////	Save current config
/////////////////////////////////////////////////////////////////////

void CUserKeys::saveConfig(void)
{
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc.LinkEndChild(decl);

	// Window attributes
	TiXmlElement *window = new TiXmlElement("window");
	window->SetAttribute("FIRE_PRIMARY", _keys[FIRE_PRIMARY]);
	window->SetAttribute("FIRE_SECONDARY", _keys[FIRE_SECONDARY]);
	window->SetAttribute("MOVE_UP", _keys[MOVE_UP]);
	window->SetAttribute("MOVE_DOWN", _keys[MOVE_DOWN]);
	window->SetAttribute("MOVE_LEFT", _keys[MOVE_LEFT]);
	window->SetAttribute("MOVE_RIGHT", _keys[MOVE_RIGHT]);

	doc.LinkEndChild(window);
	std::string path(ASSETS_PATH + "UserKeys.xml");
	doc.SaveFile(path.c_str());
}

/////////////////////////////////////////////////////////////////////
/////	Setup default keys
/////////////////////////////////////////////////////////////////////

void CUserKeys::loadDefaultConfig(void)
{
	_keys[MOVE_LEFT] = GLFW_KEY_Q;
	_keys[MOVE_RIGHT] = GLFW_KEY_D;
	_keys[MOVE_UP] = GLFW_KEY_Z;
	_keys[MOVE_DOWN] = GLFW_KEY_S;
	_keys[FIRE_PRIMARY] = GLFW_MOUSE_BUTTON_LEFT;
	_keys[FIRE_SECONDARY] = GLFW_MOUSE_BUTTON_RIGHT;
}

/////////////////////////////////////////////////////////////////////
/////	Get custom key
/////////////////////////////////////////////////////////////////////

int CUserKeys::getCustomKey(UserAction action)
{
	return _keys[action];
}

void CUserKeys::setCustomKeyPressed(int glfwCode)
{
	_customKey = glfwCode;
}

int CUserKeys::getCustomKeyPressed(void)
{
	return _customKey;
}

/////////////////////////////////////////////////////////////////////
/////	Set custom key
/////////////////////////////////////////////////////////////////////

void CUserKeys::setNewBind(UserAction action, int newBind)
{
	_keys[action] = newBind;
}

/////////////////////////////////////////////////////////////////////
/////	Check if the custom binding is a key or a button
/////////////////////////////////////////////////////////////////////

bool CUserKeys::isKey(UserAction action)
{
	int value = _keys[action];

	// Not very clean :D
	if (value >= GLFW_KEY_SPACE && value <= GLFW_KEY_LAST)
		return true;

	return false;
}

bool CUserKeys::isButton(UserAction action)
{
	int value = _keys[action];

	// Not very clean :D
	if (value >= GLFW_MOUSE_BUTTON_1 && value <= GLFW_MOUSE_BUTTON_LAST)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Utils
/////////////////////////////////////////////////////////////////////

void CUserKeys::fail(void)
{
	VC_WARNING_CRITICAL(std::string("CUserKeys::Unable to load key configuration file ") + _path +
		", using default values");
}

UserAction CUserKeys::stringToAction(const std::string &action)
{
	if (action == "FIRE_PRIMARY")
		return FIRE_PRIMARY;
	else if (action == "FIRE_SECONDARY")
		return FIRE_SECONDARY;
	else if (action == "MOVE_UP")
		return MOVE_UP;
	else if (action == "MOVE_DOWN")
		return MOVE_DOWN;
	else if (action == "MOVE_LEFT")
		return MOVE_LEFT;
	else if (action == "MOVE_RIGHT")
		return MOVE_RIGHT;

	return ACTION_MAX;
}

std::string CUserKeys::actionToString(UserAction action)
{
	if (action == FIRE_PRIMARY)
		return "FIRE_PRIMARY";
	else if (action == FIRE_SECONDARY)
		return "FIRE_SECONDARY";
	else if (action == MOVE_UP)
		return "MOVE_UP";
	else if (action == MOVE_DOWN)
		return "MOVE_DOWN";
	else if (action == MOVE_LEFT)
		return "MOVE_LEFT";
	else if (action == MOVE_RIGHT)
		return "MOVE_RIGHT";

	return "ACTION_MAX";
}

std::string CUserKeys::actionToKeyString(UserAction action)
{
	return keyToString(_keys[action]);
}

std::string CUserKeys::keyToString(int key)
{
	return _glfwStrings[key];
}

/////////////////////////////////////////////////////////////////////
/////	Map GLFW key/button to string
/////////////////////////////////////////////////////////////////////

void CUserKeys::generateGLFWStrings(void)
{
	_glfwStrings[GLFW_KEY_SPACE] = "SPACE";
	_glfwStrings[GLFW_KEY_APOSTROPHE] = "APOSTROPHE";
	_glfwStrings[GLFW_KEY_COMMA] = "COMMA";
	_glfwStrings[GLFW_KEY_MINUS] = "MINUS";
	_glfwStrings[GLFW_KEY_PERIOD] = "PERIOD";
	_glfwStrings[GLFW_KEY_SLASH] = "SLASH";
	_glfwStrings[GLFW_KEY_0] = "0";
	_glfwStrings[GLFW_KEY_1] = "1";
	_glfwStrings[GLFW_KEY_2] = "2";
	_glfwStrings[GLFW_KEY_3] = "3";
	_glfwStrings[GLFW_KEY_4] = "4";
	_glfwStrings[GLFW_KEY_5] = "5";
	_glfwStrings[GLFW_KEY_6] = "6";
	_glfwStrings[GLFW_KEY_7] = "7";
	_glfwStrings[GLFW_KEY_8] = "8";
	_glfwStrings[GLFW_KEY_9] = "9";
	_glfwStrings[GLFW_KEY_SEMICOLON] = "SEMICOLON";
	_glfwStrings[GLFW_KEY_EQUAL] = "EQUAL";
	_glfwStrings[GLFW_KEY_A] = "A";
	_glfwStrings[GLFW_KEY_B] = "B";
	_glfwStrings[GLFW_KEY_C] = "C";
	_glfwStrings[GLFW_KEY_D] = "D";
	_glfwStrings[GLFW_KEY_E] = "E";
	_glfwStrings[GLFW_KEY_F] = "F";
	_glfwStrings[GLFW_KEY_G] = "G";
	_glfwStrings[GLFW_KEY_H] = "H";
	_glfwStrings[GLFW_KEY_I] = "I";
	_glfwStrings[GLFW_KEY_J] = "J";
	_glfwStrings[GLFW_KEY_K] = "K";
	_glfwStrings[GLFW_KEY_L] = "L";
	_glfwStrings[GLFW_KEY_M] = "M";
	_glfwStrings[GLFW_KEY_N] = "N";
	_glfwStrings[GLFW_KEY_O] = "O";
	_glfwStrings[GLFW_KEY_P] = "P";
	_glfwStrings[GLFW_KEY_Q] = "Q";
	_glfwStrings[GLFW_KEY_R] = "R";
	_glfwStrings[GLFW_KEY_S] = "S";
	_glfwStrings[GLFW_KEY_T] = "T";
	_glfwStrings[GLFW_KEY_U] = "U";
	_glfwStrings[GLFW_KEY_V] = "V";
	_glfwStrings[GLFW_KEY_W] = "W";
	_glfwStrings[GLFW_KEY_X] = "X";
	_glfwStrings[GLFW_KEY_Y] = "Y";
	_glfwStrings[GLFW_KEY_Z] = "Z";
	_glfwStrings[GLFW_KEY_LEFT_BRACKET] = "LEFT_BRACKET";
	_glfwStrings[GLFW_KEY_BACKSLASH] = "BACKSLASH";
	_glfwStrings[GLFW_KEY_RIGHT_BRACKET] = "RIGHT_BRACKET";
	_glfwStrings[GLFW_KEY_GRAVE_ACCENT] = "GRAVE_ACCENT";
	_glfwStrings[GLFW_KEY_WORLD_1] = "WORLD_1";
	_glfwStrings[GLFW_KEY_WORLD_2] = "WORLD_2";
	_glfwStrings[GLFW_KEY_ESCAPE] = "ESCAPE";
	_glfwStrings[GLFW_KEY_ENTER] = "ENTER";
	_glfwStrings[GLFW_KEY_TAB] = "TAB";
	_glfwStrings[GLFW_KEY_BACKSPACE] = "BACKSPACE";
	_glfwStrings[GLFW_KEY_INSERT] = "INSERT";
	_glfwStrings[GLFW_KEY_DELETE] = "DELETE";
	_glfwStrings[GLFW_KEY_RIGHT] = "RIGHT";
	_glfwStrings[GLFW_KEY_LEFT] = "LEFT";
	_glfwStrings[GLFW_KEY_DOWN] = "DOWN";
	_glfwStrings[GLFW_KEY_UP] = "UP";
	_glfwStrings[GLFW_KEY_PAGE_UP] = "PAGE_UP";
	_glfwStrings[GLFW_KEY_PAGE_DOWN] = "PAGE_DOWN";
	_glfwStrings[GLFW_KEY_HOME] = "HOME";
	_glfwStrings[GLFW_KEY_END] = "END";
	_glfwStrings[GLFW_KEY_CAPS_LOCK] = "CAPS_LOCK";
	_glfwStrings[GLFW_KEY_SCROLL_LOCK] = "SCROLL_LOCK";
	_glfwStrings[GLFW_KEY_NUM_LOCK] = "NUM_LOCK";
	_glfwStrings[GLFW_KEY_PRINT_SCREEN] = "PRINT_SCREEN";
	_glfwStrings[GLFW_KEY_PAUSE] = "PAUSE";
	_glfwStrings[GLFW_KEY_F1] = "F1";
	_glfwStrings[GLFW_KEY_F2] = "F2";
	_glfwStrings[GLFW_KEY_F3] = "F3";
	_glfwStrings[GLFW_KEY_F4] = "F4";
	_glfwStrings[GLFW_KEY_F5] = "F5";
	_glfwStrings[GLFW_KEY_F6] = "F6";
	_glfwStrings[GLFW_KEY_F7] = "F7";
	_glfwStrings[GLFW_KEY_F8] = "F8";
	_glfwStrings[GLFW_KEY_F9] = "F9";
	_glfwStrings[GLFW_KEY_F10] = "F10";
	_glfwStrings[GLFW_KEY_F11] = "F11";
	_glfwStrings[GLFW_KEY_F12] = "F12";
	_glfwStrings[GLFW_KEY_F13] = "F13";
	_glfwStrings[GLFW_KEY_F14] = "F14";
	_glfwStrings[GLFW_KEY_F15] = "F15";
	_glfwStrings[GLFW_KEY_F16] = "F16";
	_glfwStrings[GLFW_KEY_F17] = "F17";
	_glfwStrings[GLFW_KEY_F18] = "F18";
	_glfwStrings[GLFW_KEY_F19] = "F19";
	_glfwStrings[GLFW_KEY_F20] = "F20";
	_glfwStrings[GLFW_KEY_F21] = "F21";
	_glfwStrings[GLFW_KEY_F22] = "F22";
	_glfwStrings[GLFW_KEY_F23] = "F23";
	_glfwStrings[GLFW_KEY_F24] = "F24";
	_glfwStrings[GLFW_KEY_F25] = "F25";
	_glfwStrings[GLFW_KEY_KP_0] = "KP_0";
	_glfwStrings[GLFW_KEY_KP_1] = "KP_1";
	_glfwStrings[GLFW_KEY_KP_2] = "KP_2";
	_glfwStrings[GLFW_KEY_KP_3] = "KP_3";
	_glfwStrings[GLFW_KEY_KP_4] = "KP_4";
	_glfwStrings[GLFW_KEY_KP_5] = "KP_5";
	_glfwStrings[GLFW_KEY_KP_6] = "KP_6";
	_glfwStrings[GLFW_KEY_KP_7] = "KP_7";
	_glfwStrings[GLFW_KEY_KP_8] = "KP_8";
	_glfwStrings[GLFW_KEY_KP_9] = "KP_9";
	_glfwStrings[GLFW_KEY_KP_DECIMAL] = "KP_DECIMAL";
	_glfwStrings[GLFW_KEY_KP_DIVIDE] = "KP_DIVIDE";
	_glfwStrings[GLFW_KEY_KP_MULTIPLY] = "KP_MULTIPLY";
	_glfwStrings[GLFW_KEY_KP_SUBTRACT] = "KP_SUBTRACT";
	_glfwStrings[GLFW_KEY_KP_ADD] = "KP_ADD";
	_glfwStrings[GLFW_KEY_KP_ENTER] = "KP_ENTER";
	_glfwStrings[GLFW_KEY_KP_EQUAL] = "KP_EQUAL";
	_glfwStrings[GLFW_KEY_LEFT_SHIFT] = "LEFT_SHIFT";
	_glfwStrings[GLFW_KEY_LEFT_CONTROL] = "LEFT_CONTROL";
	_glfwStrings[GLFW_KEY_LEFT_ALT] = "LEFT_ALT";
	_glfwStrings[GLFW_KEY_LEFT_SUPER] = "LEFT_SUPER";
	_glfwStrings[GLFW_KEY_RIGHT_SHIFT] = "RIGHT_SHIFT";
	_glfwStrings[GLFW_KEY_RIGHT_CONTROL] = "RIGHT_CONTROL";
	_glfwStrings[GLFW_KEY_RIGHT_ALT] = "RIGHT_ALT";
	_glfwStrings[GLFW_KEY_RIGHT_SUPER] = "RIGHT_SUPER";
	_glfwStrings[GLFW_KEY_MENU] = "MENU";
	_glfwStrings[GLFW_MOUSE_BUTTON_1] = "MOUSE_BUTTON_1";
	_glfwStrings[GLFW_MOUSE_BUTTON_2] = "MOUSE_BUTTON_2";
	_glfwStrings[GLFW_MOUSE_BUTTON_3] = "MOUSE_BUTTON_3";
	_glfwStrings[GLFW_MOUSE_BUTTON_4] = "MOUSE_BUTTON_4";
	_glfwStrings[GLFW_MOUSE_BUTTON_5] = "MOUSE_BUTTON_5";
	_glfwStrings[GLFW_MOUSE_BUTTON_6] = "MOUSE_BUTTON_6";
	_glfwStrings[GLFW_MOUSE_BUTTON_7] = "MOUSE_BUTTON_7";
	_glfwStrings[GLFW_MOUSE_BUTTON_8] = "MOUSE_BUTTON_8";
	_glfwStrings[GLFW_MOUSE_BUTTON_LAST] = "MOUSE_BUTTON_LAST";
	_glfwStrings[GLFW_MOUSE_BUTTON_LEFT] = "MOUSE_BUTTON_LEFT";
	_glfwStrings[GLFW_MOUSE_BUTTON_RIGHT] = "MOUSE_BUTTON_RIGHT";
	_glfwStrings[GLFW_MOUSE_BUTTON_MIDDLE] = "MOUSE_BUTTON_MIDDLE";
}
