/********************************************************************
**
**		InputEngine.hpp
**		Created by : Vial Joris
**
********************************************************************/

#include <iostream>
#include <SFML/Window/Joystick.hpp>
#include "InputEngine_.hpp"
#include "Log.hpp"
#include "Map.hpp"
#include "HudRessources.hpp"
#include "GraphicHelper.hpp"
#include "Event.hpp"
#include "ConfigParser.hpp"
#include "UserKeys.hpp"
#include "GUIManager.hpp"

// Define the scroll speed
#define INPUT_SCROLL_SPEED 0.05f

// Global game configuration
extern t_config	*G_conf;

// Keep a reference on the InputEngine to be used in callbacks
static InputEngine_ *inputEngineRef = NULL;

// Global camera for coordinate conversion
namespace VoidClashGraphics {
	extern ogl::Camera globalCamera;
}

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

InputEngine_::InputEngine_(void)
{
	_win = NULL;

	// Keep a refence on myself for callbacks
	inputEngineRef = this;

	// Create menu shortcuts
	_menuShortcuts[GLFW_KEY_0] = CEGUI::Key::Zero;
	_menuShortcuts[GLFW_KEY_1] = CEGUI::Key::One;
	_menuShortcuts[GLFW_KEY_2] = CEGUI::Key::Two;
	_menuShortcuts[GLFW_KEY_3] = CEGUI::Key::Three;
	_menuShortcuts[GLFW_KEY_4] = CEGUI::Key::Four;
	_menuShortcuts[GLFW_KEY_5] = CEGUI::Key::Five;
	_menuShortcuts[GLFW_KEY_6] = CEGUI::Key::Six;
	_menuShortcuts[GLFW_KEY_7] = CEGUI::Key::Seven;
	_menuShortcuts[GLFW_KEY_8] = CEGUI::Key::Eight;
	_menuShortcuts[GLFW_KEY_9] = CEGUI::Key::Nine;
	_menuShortcuts[GLFW_KEY_KP_0] = CEGUI::Key::Zero;
	_menuShortcuts[GLFW_KEY_KP_1] = CEGUI::Key::One;
	_menuShortcuts[GLFW_KEY_KP_2] = CEGUI::Key::Two;
	_menuShortcuts[GLFW_KEY_KP_3] = CEGUI::Key::Three;
	_menuShortcuts[GLFW_KEY_KP_4] = CEGUI::Key::Four;
	_menuShortcuts[GLFW_KEY_KP_5] = CEGUI::Key::Five;
	_menuShortcuts[GLFW_KEY_KP_6] = CEGUI::Key::Six;
	_menuShortcuts[GLFW_KEY_KP_7] = CEGUI::Key::Seven;
	_menuShortcuts[GLFW_KEY_KP_8] = CEGUI::Key::Eight;
	_menuShortcuts[GLFW_KEY_KP_9] = CEGUI::Key::Nine;
	_joypadIndex = 0;
}

InputEngine_::~InputEngine_(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Initialization
/////////////////////////////////////////////////////////////////////

void InputEngine_::start(void)
{
	VC_INFO("InputEngine::starting");

	// Set callbacks. Use only this callbacks for CEGUI !
	// For others events, add functions in InputEngine::update
	GLFWwindow *win = &(S_Context->getWindow());
	_guiKeyCallback = glfwSetKeyCallback(win, injectKeyToCEGUI);
	_guiMouseCallback = glfwSetMouseButtonCallback(win, injectMouseToCEGUI);
	_guiMouseMoveCallback = glfwSetCursorPosCallback(win, injectMouseMoveToCEGUI);
	_guiMouseScrollCallback = glfwSetScrollCallback(win, injectMouseScrollToCEGUI);
	_guiTextInputCallback = glfwSetCharCallback(win, injectTextInputToCEGUI);

	_gameState = RUN;

	VC_INFO("InputEngine::started");
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

eGameState InputEngine_::update(const sf::Time &deltatime)
{
	(void)deltatime;

	// Get window datas
	_win = &(S_Context->getWindow());

	// Save mouse position for this frame
	checkJoypadIndex();
	if (!sf::Joystick::isConnected(_joypadIndex))
	{
		double mousePX, mousePY;
		glfwGetCursorPos(&(S_Context->getWindow()), &mousePX, &mousePY);
		HudRessources::getInstance()->setMousePosition(sf::Vector2i(mousePX, mousePY));
	}

	// Check if we need to exit the game
	if (checkExitCondition() == true)
		return (_gameState = EXIT);

	// Clipboard keys - Needed by the GUI
	checkClipboardKeys();

	// Check events which could change the game state
	checkGameStateInput();

	// Ignore all event that could affect the gameplay
	// if we don't need to check them
	if (ignoreFrameEvents() == true)
	{
		resetPlayerInputs();
		sendEventAction();
		return (_gameState = RUN);
	}

	// Check player inputs
	checkPlayerInputs();

	return _gameState;
}

void	InputEngine_::checkJoypadIndex()
{
	sf::Joystick::update();
	_joypadIndex = 0;
	for (int index = 0; index <= sf::Joystick::Count; ++index)
	{
		if (sf::Joystick::isConnected(index))
			_joypadIndex = index;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Check if we need to go throught the InputEngine
/////////////////////////////////////////////////////////////////////

bool InputEngine_::ignoreFrameEvents(void)
{
	// We don't have focus
	if (glfwGetWindowAttrib(_win, GLFW_FOCUSED) == 0)
		return true;

	// GUI currently displayed
	if (S_GUI->isDisplayed() == true)
		return true;

	// Chat input enabled
	if (S_GUI->chatInputEnabled() == true)
		return true;

	// No maps are active
	if (S_Map->isMapActive() == false)
		return true;

	// Current player is dead
	if (S_Map->getCurrentPlayer() &&
		S_Map->getCurrentPlayer()->isRespawning() == true)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Check EXIT condition
/////////////////////////////////////////////////////////////////////

bool InputEngine_::checkExitCondition(void)
{
	// TODO - Remove it for release
	int state = glfwGetKey(_win, GLFW_KEY_F2);
	if (state == GLFW_PRESS)
		return true;

	if (Event::getEventByType(ev_QUIT) != NULL)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Send player action event
/////////////////////////////////////////////////////////////////////

void InputEngine_::sendEventAction(void)
{
	ADD_EVENT(ev_PLAYER_ACTION,
		s_event(S_Map->getCurrentPlayer(), &_playerActions));
}

/////////////////////////////////////////////////////////////////////
/////	Player inputs
/////////////////////////////////////////////////////////////////////

void InputEngine_::resetPlayerInputs(void)
{
	float prevAimX = _playerActions.aimX;
	float prevAimY = _playerActions.aimY;

	memset(&_playerActions, 0, sizeof(s_actions));

	_playerActions.aimX = prevAimX;
	_playerActions.aimY = prevAimY;
}

void InputEngine_::checkPlayerInputs(void)
{
	// Reset
	resetPlayerInputs();

	// No in game input if the menu is open
	if (_gameState == MENU)
		return;

	// Do not take inputs if player is respawning
	if (S_Map->getCurrentPlayer() && S_Map->getCurrentPlayer()->isRespawning())
		return;

	// Movement
	checkPlayerMove();

	// Aim direction
	checkAimPos();

	// Mouse inputs
	checkFire();

	// Check switches
	checkPlayerSwitches();

	// Send actions
	sendEventAction();
}

void InputEngine_::checkPlayerMove(void)
{
	if (sf::Joystick::isConnected(_joypadIndex))
	{
		if (std::abs(sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::X)) > 40.f || std::abs(sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::Y)) > 40.f)
		{
			_playerActions.moveX = sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::X);
			_playerActions.moveY = sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::Y);
		}
		return;
	}
	if (checkCustomBinding(MOVE_LEFT))
		_playerActions.moveX -= 100;
	if (checkCustomBinding(MOVE_RIGHT))
		_playerActions.moveX += 100;
	if (checkCustomBinding(MOVE_UP))
		_playerActions.moveY -= 100;
	if (checkCustomBinding(MOVE_DOWN))
		_playerActions.moveY += 100;
}

// TODO joypad
void InputEngine_::checkAimPos(void)
{
	// Do not lock position when scoreboard or menu
	if (S_GUI->scoreboardDisplayed() || S_GUI->isDisplayed())
		return;

	// Joypad
	auto width = S_Context->getWindowSize().x;
	auto height = S_Context->getWindowSize().y;
	static float saveX = 100.f;
	static float saveY = 0.f;
	if (sf::Joystick::isConnected(_joypadIndex))
	{
		const std::shared_ptr<Player> &p = S_Map->getCurrentPlayer();
		if (p)
		{
			const auto& ident = sf::Joystick::getIdentification(_joypadIndex);
			if (ident.productId == 654) // Xbox special treatment
			{
				if (std::abs(sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::R)) > 40.f || std::abs(sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::U)) > 40.f)
				{
					saveX = sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::U);
					saveY = sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::R);
				}
			}
			else
			{
				if (std::abs(sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::R)) > 40.f || std::abs(sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::Z)) > 40.f)
				{
					saveX = sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::R);
					saveY = sf::Joystick::getAxisPosition(_joypadIndex, sf::Joystick::Z);
				}
			}

			HudRessources::getInstance()->setMousePosition(sf::Vector2i(width / 2 + saveX, height / 2 + saveY));
		}
	}

	// Lock cursor
	const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());
	if (AIM_LOCK && !sf::Joystick::isConnected(_joypadIndex))
	{
		float distance = std::sqrt(std::pow(width / 2 - mouse.x, 2) + std::pow(height / 2 - mouse.y, 2)) + 1;
		if (distance > AIM_LOCK_DIST * (100 + AIM_LOCK_BUFFER_PERCENT) / 100 ||
			distance < AIM_LOCK_DIST * (100 - AIM_LOCK_BUFFER_PERCENT) / 100)
		{
			float goalPosX = width / 2 - (width / 2 - mouse.x) / distance * AIM_LOCK_DIST;
			float goalPosY = height / 2 - (height / 2 - mouse.y) / distance * AIM_LOCK_DIST;
			float newPosX = (goalPosX + AIM_LOCK_SMOOTH_POWER * mouse.x) / (AIM_LOCK_SMOOTH_POWER + 1);
			float newPosY = (goalPosY + AIM_LOCK_SMOOTH_POWER * mouse.y) / (AIM_LOCK_SMOOTH_POWER + 1);
			glfwSetCursorPos(&(S_Context->getWindow()), newPosX, newPosY);
			HudRessources::getInstance()->setMousePosition(sf::Vector2i(newPosX, newPosY));
		}
	}

	VoidClashGraphics::screenToWorld(
		mouse.x, mouse.y,
		_playerActions.aimX, _playerActions.aimY,
		&VoidClashGraphics::globalCamera
	);
}

void InputEngine_::checkFire(void)
{
	if (sf::Joystick::isConnected(_joypadIndex))
	{
		_playerActions.primary = sf::Joystick::isButtonPressed(_joypadIndex, 4);
		_playerActions.secondary = sf::Joystick::isButtonPressed(_joypadIndex, 5);
		return;
	}
	if (checkCustomBinding(FIRE_PRIMARY))
		_playerActions.primary = true;

	if (checkCustomBinding(FIRE_SECONDARY))
		_playerActions.secondary = true;
}

void InputEngine_::checkPlayerSwitches(void)
{
	// TODO if we put the weapon switch again
}

/////////////////////////////////////////////////////////////////////
/////	Check the clipboard keys
/////////////////////////////////////////////////////////////////////

void InputEngine_::checkClipboardKeys(void)
{
	// Left Ctrl
	int stateKeyCtrl = glfwGetKey(_win, GLFW_KEY_LEFT_CONTROL);

	// C
	int stateKeyC = glfwGetKey(_win, GLFW_KEY_C);

	// V
	int stateKeyV = glfwGetKey(_win, GLFW_KEY_V);

	// X
	int stateKeyX = glfwGetKey(_win, GLFW_KEY_X);

	// Check clipboard combinaisons
	if (stateKeyCtrl == GLFW_PRESS)
	{
		CEGUI::GUIContext& context = CEGUI_SYSTEM.getDefaultGUIContext();

		if (stateKeyC == GLFW_PRESS)
			context.injectCopyRequest();
		else if (stateKeyV == GLFW_PRESS)
			context.injectPasteRequest();
		else if (stateKeyX == GLFW_PRESS)
			context.injectCutRequest();
	}
}

/////////////////////////////////////////////////////////////////////
/////	Check inputs according to the user custom binding
/////////////////////////////////////////////////////////////////////

bool InputEngine_::checkCustomBinding(UserAction action)
{
	// Keyboard
	if (S_Keys->isKey(action) &&
		glfwGetKey(_win, S_Keys->getCustomKey(action)) == GLFW_PRESS)
	{
		return true;
	}

	// Keyboard
	if (S_Keys->isButton(action) &&
		glfwGetMouseButton(_win, S_Keys->getCustomKey(action)) == GLFW_PRESS)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Other inputs (can be used for debug purpose)
/////////////////////////////////////////////////////////////////////

void InputEngine_::checkOtherInputs(void)
{

}

/////////////////////////////////////////////////////////////////////
/////	Check events which could change the game state
/////////////////////////////////////////////////////////////////////

eGameState InputEngine_::checkGameStateInput(void)
{
	// Scoreboard
	if (glfwGetKey(_win, GLFW_KEY_TAB) == GLFW_PRESS)
		S_GUI->displayScoreboard(true);
	else if (glfwGetKey(_win, GLFW_KEY_TAB) == GLFW_RELEASE)
		S_GUI->displayScoreboard(false);

	return _gameState;
}

/////////////////////////////////////////////////////////////////////
/////	Inject GLFW event into CEGUI <3
////	Here We use a callback to handle every type of event
////	since we don't care about the order of events in the GUI
/////////////////////////////////////////////////////////////////////

///////// Keyboard /////////

void injectKeyToCEGUI(GLFWwindow* window, int key, int scancode,
	int action, int mods)
{
	(void)window;
	(void)scancode;
	(void)mods;

	// Update S_Keys
	S_Keys->setCustomKeyPressed(key);

	CEGUI::GUIContext& context = CEGUI_SYSTEM.getDefaultGUIContext();

	// Menus shortcuts
	if (action == GLFW_RELEASE)
	{
		// Check if the current key is a binded shortcut (created in the Ctor)
		std::map<int, CEGUI::Key::Scan> &_menuShortcuts = inputEngineRef->getMenuShortcuts();
		auto it = _menuShortcuts.find(key);
		if (it != _menuShortcuts.end())
			context.injectKeyUp(it->second);
	}

	// Enable - Disable GUI modules
	if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER)
		S_GUI->injectModuleActivationKey(CEGUI::Key::Return);
	if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE)
		S_GUI->injectModuleActivationKey(CEGUI::Key::Escape);
	if (action == GLFW_RELEASE && key == GLFW_KEY_N)
		S_GUI->injectModuleActivationKey(CEGUI::Key::N);
	if (action == GLFW_RELEASE && key == GLFW_KEY_B)
		S_GUI->injectModuleActivationKey(CEGUI::Key::B);

	// Inject keys which will be catched by "GUIManager::enabledModules"
	if (key == GLFW_KEY_ENTER)
	{
		if (action == GLFW_RELEASE)
			context.injectKeyUp(CEGUI::Key::Return);
		else if (action == GLFW_PRESS)
			context.injectKeyDown(CEGUI::Key::Return);
	}
	if (key == GLFW_KEY_ESCAPE)
	{
		if (action == GLFW_RELEASE)
			context.injectKeyUp(CEGUI::Key::Escape);
	}
	if (key == GLFW_KEY_BACKSPACE)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			context.injectKeyDown(CEGUI::Key::Backspace);
	}
	if (key == GLFW_KEY_LEFT)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			context.injectKeyDown(CEGUI::Key::ArrowLeft);
	}
	if (key == GLFW_KEY_RIGHT)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			context.injectKeyDown(CEGUI::Key::ArrowRight);
	}
}

///////// Mouse button /////////

void injectMouseToCEGUI(GLFWwindow* window, int button, int action, int mods)
{
	(void)window;
	(void)mods;

	// Update S_Keys
	S_Keys->setCustomKeyPressed(button);

	CEGUI::GUIContext& context = CEGUI_SYSTEM.getDefaultGUIContext();

	// Left button
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			context.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
		else if (action == GLFW_RELEASE)
			context.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}

	// Right button
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
			context.injectMouseButtonDown(CEGUI::MouseButton::RightButton);
		else if (action == GLFW_RELEASE)
			context.injectMouseButtonUp(CEGUI::MouseButton::RightButton);
	}
}

///////// Mouse move /////////

void injectMouseMoveToCEGUI(GLFWwindow* window, double xpos, double ypos)
{
	(void)window;

	CEGUI_SYSTEM.getDefaultGUIContext().
		injectMousePosition(xpos, ypos);
}

///////// Mouse scroll /////////

void injectMouseScrollToCEGUI(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;

	if (G_conf == NULL)
		return;

	// If we are in the menus or chat, inject into CEGUI
	if (S_GUI->isDisplayed() || S_GUI->chatInputEnabled() == true)
	{
		CEGUI_SYSTEM.getDefaultGUIContext().
			injectMouseWheelChange(yoffset);
		return;
	}

	// We are in game, update the camera zoom;
	float current = S_Map->getZoom();

	current -= yoffset * INPUT_SCROLL_SPEED;
	current = std::min(G_conf->game->zoom, current);
	current = std::max(0.25f, current);

	S_Map->setZoom(current);
}

///////// Text input /////////

void injectTextInputToCEGUI(GLFWwindow* window, unsigned int codepoint)
{
	(void)window;

	CEGUI_SYSTEM.getDefaultGUIContext().
		injectChar(codepoint);
}

///////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void InputEngine_::stop(void)
{
	VC_INFO("InputEngine::stopping");
	VC_INFO("InputEngine::stopped");
}
