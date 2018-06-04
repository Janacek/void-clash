#pragma once

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <CEGUI/CEGUI.h>

#include "GUICallbacks.h"

#include "MapData.h"

class Application
{
public:
	Application(sf::Window *);
	~Application();

	void Init();
	void Run();
	void Update();
	void Draw();

	MapData *getMapData();

	void NewMap();
	void SaveMap();
	void OpenMap(const std::string &);
	void CancelOpen();

	void ShowWarning(const std::string &);

	void ApplyNewValues();

	std::string				mapToLoad;

private:
	sf::Window	*m_window;


	CEGUI::Window		*myRoot;
	CEGUI::OpenGLRenderer *m_renderer;

	GUICallbacks m_guiCallbacks;

	CEGUI::Listbox			*mapItems;

	/*
	** MapData.
	*/
	MapData					*m_mapData;

}; 