#pragma once

#include <CEGUI/CEGUI.h>
#include <iostream>

#include <map>
#include "Map.hpp"

//#include "Application.h"
class GUIEngine;

class GUICallbacks
{
public:
	GUICallbacks();
	~GUICallbacks();


	bool PrintHello(const CEGUI::EventArgs &);
	bool ItemSelectionChanged(const CEGUI::EventArgs &e);
	void OpenMapThreadFunc();
	void SaveAsMapThreadFunc();
	bool OpenMap(const CEGUI::EventArgs &);
	bool OpenMapLoad(const CEGUI::EventArgs &);
	bool ConfirmMapToLoad(const CEGUI::EventArgs &);
	bool CancelLoad(const CEGUI::EventArgs &);
	bool SaveAsMap(const CEGUI::EventArgs &);
	bool SaveAsMap();
	bool SaveMap(const CEGUI::EventArgs &);
	bool NewMap(const CEGUI::EventArgs &);

	bool SaveAsMap1(const CEGUI::EventArgs &);

	bool NewItemSelected(const CEGUI::EventArgs &);

	bool DeleteObject(const CEGUI::EventArgs &);

	bool WallCreationSelection(const CEGUI::EventArgs &);

	void setApp(GUIEngine *app);

	//void setApp(GUIEngine *app);
	//void setApp(GUIEngine *app);

	bool XEditBoxChanged(const CEGUI::EventArgs &);
	bool YEditBoxChanged(const CEGUI::EventArgs &);
	bool XDirEditBoxChanged(const CEGUI::EventArgs &);
	bool YDirEditBoxChanged(const CEGUI::EventArgs &);
	bool XEndEditBoxChanged(const CEGUI::EventArgs &);
	bool YEndEditBoxChanged(const CEGUI::EventArgs &);
	bool WidthEditBoxChanged(const CEGUI::EventArgs &);
	bool HeightEditBoxChanged(const CEGUI::EventArgs &);
	bool PowerEditBoxChanged(const CEGUI::EventArgs &);
	bool TeamEditBoxChanged(const CEGUI::EventArgs &);
	bool SizeEditBoxChanged(const CEGUI::EventArgs &);

	void EditBoxChanged();

	bool HoverMenu(const CEGUI::EventArgs &);
	bool NotHoverMenu(const CEGUI::EventArgs &);

	std::map<sf::Keyboard::Key, CEGUI::Key::Scan>	toCEGUIkeys;

	bool											multiSelectionEnabled;



	GUIEngine									*m_app;
};