#pragma once

#ifndef MAPDATA_H__
#define MAPDATA_H__

#include <string>

#define BASICMAPNAME "Sans Titre"
#include "MapStructures.h"

#include <CEGUI/CEGUI.h>

class MapData
{
public:
	MapData();
	MapData(const std::string &, const std::string &);
	~MapData();

	void LoadMap();
	void LoadMapGUI();

	/*
	** GETTER & SETTER
	*/
	void setName(const std::string &);
	const std::string &getName();

	void setFullPath(const std::string &);
	const std::string &getFullPath();

	void setModifications(bool);
	bool getModifications();

	void setObjectParameters(int id);
	void setVisibleParameters(char *parameters);

	void applyNewValues();

	t_map			*_map;


//private:
	std::string		m_name;
	std::string		m_fullPath;

	int				m_nbOfItems;
	bool			m_modificationsButNotSaved;
};

#endif