/********************************************************************
**
**		KeyPreferences.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef KEY_PREFERENCES
#define KEY_PREFERENCES

#include "GUIEntity.hpp"

class KeyPreferences : public GUIEntity
{
public:
	KeyPreferences(void);
	virtual ~KeyPreferences(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

private:
	void setupKeyList(void);
	void updateText(CEGUI::ListboxTextItem *item, std::string text);
	void refreshListBox(void);

	// Change custom binding
	void changeKey(CEGUI::ListboxTextItem *item, const std::string label);

	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
	bool onButtonReleased(const CEGUI::EventArgs &eIN);
	bool onKeyReleased(const CEGUI::EventArgs &eIN);
	bool onCharKey(const CEGUI::EventArgs &eIN);
	bool onEventSelectionChanged(const CEGUI::EventArgs &eIN);

private:
	bool _changingKey;
	bool _ingoreNextKeyUp;
	
	CEGUI::ListboxTextItem *_keyToChange;
};

#endif