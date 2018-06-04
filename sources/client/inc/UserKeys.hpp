/********************************************************************
**
**		UserKeys.hpp
**		Created by : Vial Joris
**
********************************************************************/


#ifndef USER_KEYS
#define USER_KEYS

#include <map>
// Fix linking bugs on Linux
#ifndef _WIN32
#define TIXML_USE_STL
#endif
#include <tinyxml.h>
#include "SingletonBase.hpp"
#include "InputEngine_.hpp"

class CUserKeys
{
public:
	CUserKeys(void);
	~CUserKeys(void);

	void start(void) {}
	void loadConfig(void);
	void setConfigFilePath(const std::string &path);
	void stop(void);

	// Save current config
	void saveConfig(void);

	// Return GLFW_KEY_XXX
	int getCustomKey(UserAction action);
	bool isKey(UserAction action);
	bool isButton(UserAction action);

	// Update custom binding
	void setCustomKeyPressed(int glfwCode);
	int getCustomKeyPressed(void);
	void setNewBind(UserAction action, int newBind);

	// Convertion utils
	UserAction stringToAction(const std::string &action);
	std::string actionToString(UserAction action);
	std::string keyToString(int key);
	std::string actionToKeyString(UserAction action);

private:
	// Config loading
	void loadKeys(void);
	bool loadKey(const std::string &name, TiXmlElement *e);
	void loadDefaultConfig(void);

	// Map GLFW key/button to string
	void generateGLFWStrings(void);

	// Utils
	void fail(void);

private:
	// User is defining key
	bool _active;

	// UserAction - GLFW_KEY_XXX
	std::map<UserAction, int> _keys;

	// Config file path
	std::string _path;

	// Map GLFW key/button to string
	std::map<int, std::string> _glfwStrings;

	// Key/Button pressed while settings custom binding
	int _customKey;
};

typedef SingletonBase<CUserKeys> UserKeysSingleton;
#define S_Keys UserKeysSingleton::self()

#endif
