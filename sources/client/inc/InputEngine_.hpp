/********************************************************************
**
**		InputEngine.hpp
**		Created by : Vial Joris
**
********************************************************************/

#ifndef INPUT_ENGINE
#define INPUT_ENGINE

#include "Context.hpp"
#include "AEngine.hpp"
#include "Actions.hpp"
#include "CEGUI/CEGUI.h"

#define		AIM_LOCK		true		// If enabled, mouse position will be limited to the defined shape
#define		AIM_LOCK_DIST	300.f		// Shape radius
#define		AIM_LOCK_SMOOTH_POWER	0	// 0 = no smooth - Smooth the push back of the mouse
//#define		AIM_LOCK_INSIDE	false		// If enabled, the shape will not be filled inside
#define		AIM_LOCK_BUFFER_PERCENT	100  // 0 = no buffer - When AIM_LOCK_INSIDE is false,

enum UserAction
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_UP,
	FIRE_PRIMARY,
	FIRE_SECONDARY,
	ACTION_MAX,
};

class InputEngine_ : public AEngine
{
public:
	InputEngine_(void);
	virtual ~InputEngine_(void);

	virtual void start(void);
	virtual void stop(void);
	virtual eGameState update(const sf::Time &deltaTime);

	// Setters
	void setGameState(eGameState gameState) { _gameState = gameState; }

	// Getter used by the reference on the static InputEngine
	// Designed to be used inside callback functions and nowhere else !
	std::map<int, CEGUI::Key::Scan>& getMenuShortcuts(void) { return _menuShortcuts; }
	eGameState& getGameState(void) { return _gameState; }

private:
	// Check if we need to go throught the InputEngine
	bool ignoreFrameEvents(void);

	// EXIT condition
	bool checkExitCondition(void);

	// Player events
	void sendEventAction(void);
	void resetPlayerInputs(void);
	void checkPlayerInputs(void);
	void checkPlayerMove(void);
	void checkFire(void);
	void checkPlayerSwitches(void);
	void checkAimPos(void);
	bool checkCustomBinding(UserAction action);

	// GUI events
	void checkClipboardKeys(void);

	// Check events which could change the game state
	eGameState checkGameStateInput(void);

	// Other events (can be used for debug purpose)
	void checkOtherInputs(void);

	// Inject GLFW event into CEGUI <3
	void injectToCEGUI(void);

	void	checkJoypadIndex();

private:
	// Keep datas about window
	GLFWwindow *_win;
	int _hasFocus;

	// Callbacks
	GLFWkeyfun _guiKeyCallback;
	GLFWmousebuttonfun _guiMouseCallback;
	GLFWcursorposfun _guiMouseMoveCallback;
	GLFWscrollfun _guiMouseScrollCallback;
	GLFWcharfun _guiTextInputCallback;

	// Current game state
	eGameState	_gameState;

	// Player actions
	s_actions _playerActions;

	// Main menu shortcuts
	std::map<int, CEGUI::Key::Scan> _menuShortcuts;

	int	_joypadIndex; // -1 if none
};

// Callbacks
void injectKeyToCEGUI(GLFWwindow* window, int key, int scancode, int action, int mods);
void injectMouseToCEGUI(GLFWwindow* window, int button, int action, int mods);
void injectMouseMoveToCEGUI(GLFWwindow* window, double xpos, double ypos);
void injectMouseScrollToCEGUI(GLFWwindow* window, double xoffset, double yoffset);
void injectTextInputToCEGUI(GLFWwindow* window, unsigned int codepoint);

#endif
