#include "MapData.h"
#include "MapParser.hpp"

#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <CEGUI/CEGUI.h>
#include <map>

#include "CustomListBoxItem.h"

#include "Map.hpp"

MapData::MapData()
{
	m_modificationsButNotSaved = false;
	m_name = BASICMAPNAME;
	m_fullPath = "";
}

MapData::MapData(const std::string &name, const std::string &fullPath)
: m_name(name), m_fullPath(fullPath)
{
	m_modificationsButNotSaved = false;
}

MapData::~MapData()
{

}

void MapData::setName(const std::string &name)
{
	m_name = name;
}

const std::string &MapData::getName()
{
	return m_name;
}

void MapData::setFullPath(const std::string &fullPath)
{
	m_fullPath = fullPath;
}

const std::string &MapData::getFullPath()
{
	return m_fullPath;
}

void MapData::setModifications(bool v)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	CEGUI::Window *save = myRoot->getChildRecursive("SaveButton");
	CEGUI::Window *saveAs = myRoot->getChildRecursive("SaveAsButton");

	if (v)
	{
		save->setDisabled(false);
		saveAs->setDisabled(false);
	}
	else
	{
		save->setDisabled(true);
		saveAs->setDisabled(true);
	}

	m_modificationsButNotSaved = v;

}

bool MapData::getModifications()
{
	return m_modificationsButNotSaved;
}

void MapData::setVisibleParameters(char *parameters)
{
	std::map<int, char *> m;
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Window *deleteButton = myRoot->getChildRecursive("deleteObjectButton");

	
	if (std::string(parameters) == "00000000000")
		deleteButton->setVisible(false);
	else
		deleteButton->setVisible(true);

	m[0] = "XText";
	m[1] = "YText";
	m[2] = "endXText";
	m[3] = "endYText";
	m[4] = "dirXText";
	m[5] = "dirYText";
	m[6] = "widthText";
	m[7] = "heightText";
	m[8] = "teamText";
	m[9] = "sizeText";
	m[10] = "power";

	for (int i = 0; i < 11; ++i)
	{
		CEGUI::Window *param = myRoot->getChildRecursive(m[i]);
		if (parameters[i] == '1')
			param->setVisible(true);
		else
			param->setVisible(false);
	}
}

#include <string>
#include <iostream>

void MapData::setObjectParameters(int id)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	CEGUI::Window *X = myRoot->getChildRecursive("X");
	CEGUI::Window *Y = myRoot->getChildRecursive("Y");
	CEGUI::Window *endX = myRoot->getChildRecursive("endX");
	CEGUI::Window *endY = myRoot->getChildRecursive("endY");
	CEGUI::Window *dirX = myRoot->getChildRecursive("dirX");
	CEGUI::Window *dirY = myRoot->getChildRecursive("dirY");
	CEGUI::Window *width = myRoot->getChildRecursive("width");
	CEGUI::Window *height = myRoot->getChildRecursive("height");
	CEGUI::Window *team = myRoot->getChildRecursive("team");
	CEGUI::Window *size = myRoot->getChildRecursive("size");
	CEGUI::Window *power = myRoot->getChildRecursive("power");

	AObject *object = S_Map->findObjectWithID(id).get();
	object->setSelected(true);

	if (object->getType() == RESPAWN)
	{
		setVisibleParameters("11000011100");
		X->setText(std::to_string(object->getX()));
		Y->setText(std::to_string(object->getY()));
		width->setText(std::to_string(object->getWidth()));
		height->setText(std::to_string(object->getHeight()));
		team->setText(std::to_string(object->getTeam()));
	}
	else if (object->getType() == WALL)
	{
		setVisibleParameters("11110000000");
		X->setText(std::to_string(object->getX()));
		Y->setText(std::to_string(object->getY()));
		endX->setText(std::to_string(object->getEndX()));
		endY->setText(std::to_string(object->getEndY()));
	}
	else if (object->getType() == SPEED_FIELD)
	{
		setVisibleParameters("11000011000");
		X->setText(std::to_string(object->getX()));
		Y->setText(std::to_string(object->getY()));
		width->setText(std::to_string(object->getWidth()));
		height->setText(std::to_string(object->getHeight()));
	}
	else if (object->getType() == CAPTURE)
	{
		setVisibleParameters("11000011000");
		X->setText(std::to_string(object->getX()));
		Y->setText(std::to_string(object->getY()));
		width->setText(std::to_string(object->getWidth()));
		height->setText(std::to_string(object->getHeight()));
	}
	else if (object->getType() == FLAG)
	{
		setVisibleParameters("11000000100");
		X->setText(std::to_string(object->getX()));
		Y->setText(std::to_string(object->getY()));
		team->setText(std::to_string(object->getTeam()));
	}


	if (m_modificationsButNotSaved == false)
	{
		CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

		CEGUI::Window *save = myRoot->getChildRecursive("SaveButton");
		CEGUI::Window *saveAs = myRoot->getChildRecursive("SaveAsButton");

		save->setDisabled(true);
		saveAs->setDisabled(true);
	}
}

void MapData::applyNewValues()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));
	int id = box->getFirstSelectedItem()->getID();
}

void MapData::LoadMapGUI()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	box->resetList();

	m_nbOfItems = 0;

	for (auto it = S_Map->getElems()->begin(); it != S_Map->getElems()->end(); ++it)
	{
		std::string name;

		if ((*it)->getType() == RESPAWN)
			name = "Respawn";
		else if ((*it)->getType() == FLAG)
			name = "Flag";
		else if ((*it)->getType() == SPEED_FIELD)
			name = "Speed field";
		else if ((*it)->getType() == CAPTURE)
			name = "Capture Zone";

		CustomListBoxItem *entry = new CustomListBoxItem(name.c_str(), NULL, (*it)->getId());
		entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
		box->addItem(entry);
		++m_nbOfItems;
	}

	for (auto it = S_Map->getWalls()->begin(); it != S_Map->getWalls()->end(); ++it)
	{
		CustomListBoxItem *entry = new CustomListBoxItem("Wall", NULL, (*it)->getId());
		entry->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
		box->addItem(entry);
		++m_nbOfItems;
	}
}

void MapData::LoadMap()
{
	S_Map->changeMap(m_fullPath.c_str());
}