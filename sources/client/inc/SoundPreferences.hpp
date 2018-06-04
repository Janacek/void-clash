/********************************************************************
**
**		SoundPreferences.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef SOUND_PREFERENCES
#define SOUND_PREFERENCES

#include "GUIEntity.hpp"

class SoundPreferences : public GUIEntity
{
public:
	SoundPreferences(void);
	virtual ~SoundPreferences(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

private:
	// Add button and bind callbacks(could be templated ? )
	void addButton(const std::string &name, bool hasHover = false);

	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);
	bool onEventScrollPositionChanged(const CEGUI::EventArgs &eIN);
	bool onEventCheckStateChanged(const CEGUI::EventArgs &eIN);
};

#endif


