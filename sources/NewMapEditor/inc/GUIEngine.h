#ifndef GUIENGINE_H__
#define GUIENGINE_H__

#include <Context.hpp>

#include "AEngine.hpp"
#include	"Map.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <CEGUI/CEGUI.h>

#include "GUICallbacks.h"

#include "MapData.h"

#include "CustomListBoxItem.h"

class Manager;

class GUIEngine : public AEngine
{
public:
	GUIEngine();
	virtual ~GUIEngine(void);

	// IEngine
	virtual void start(void);
	virtual void stop(void);
	virtual eGameState update(const sf::Time &deltaTime);
	virtual eGameState update(const sf::Time &deltaTime, bool);

	MapData *getMapData();
	void NewMap();
	void SaveMap();
	void OpenMap(const std::string &);
	void LoadMapGUI();
	void OpenMapWindow();
	void ConfirmMapToLoad();
	void CancelOpen();

	void SelectObjectById(int);

	void CreateNewObject(const std::string &);

	void ShowWarning(const std::string &);

	void ApplyNewValues();

	void InitLoadMaps();
	void InitAvailableItems();

	std::string				mapToLoad;
	bool					hoverMenu;
	eObjectType				_objectToCreate;
	AObject					*_objectCreated;

private:
	sf::Window				*m_window;


	CEGUI::Window			*myRoot;
	CEGUI::OpenGLRenderer	*m_renderer;

	GUICallbacks			m_guiCallbacks;

	CEGUI::Listbox			*mapItems;

	/*
	** MapData.
	*/
	MapData					*m_mapData;

	bool					_mapLoaded;

	void createWall(float, float, CEGUI::Listbox *);
	void createRespawn(float, float, CEGUI::Listbox *);
	void createCapture(float, float, CEGUI::Listbox *);
	void createFlag(float, float, CEGUI::Listbox *);
};

#endif