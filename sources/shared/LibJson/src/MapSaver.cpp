#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Wall.hpp"
#include "MapSaver.h"
#include "Map.hpp"

void MapSaver::saveMap(t_map *map, const std::string &name, const std::string &fullName)
{
	(void)map;
	
	Json::Value fromScratch;

	fromScratch["map"] = name;
	Json::Value gravityFields;
	Json::Value wallsNoDir;
	Json::Value flags;
	Json::Value speed_fields;
	Json::Value spawns;
	Json::Value capture;

	bool brespawns = false;
	bool bflags = false;
	bool bspeedFields = false;
	bool bwalls = false;
	bool bcapture = false;

	for (auto it = S_Map->getElems()->begin(); it != S_Map->getElems()->end(); ++it)
	{
		Json::Value array;
		AObject *tmp = (*it).get();

		if ((*it)->getType() == RESPAWN)
		{
			array.append(tmp->getX());
			array.append(tmp->getY());
			array.append(tmp->getDirX());
			array.append(tmp->getDirY());
			array.append(tmp->getWidth());
			array.append(tmp->getHeight());
			array.append(tmp->getTeam());

			spawns.append(array);
			brespawns = true;
		}
		else if ((*it)->getType() == FLAG)
		{
			array.append(tmp->getX());
			array.append(tmp->getY());
			array.append(tmp->getDirX());
			array.append(tmp->getDirY());
			array.append(tmp->getTeam());

			flags.append(array);
			bflags = true;
		}
		else if ((*it)->getType() == SPEED_FIELD)
		{
			array.append(tmp->getX());
			array.append(tmp->getY());
			array.append(tmp->getDirX());
			array.append(tmp->getDirY());
			array.append(tmp->getWidth());
			array.append(tmp->getHeight());

			speed_fields.append(array);
			bspeedFields = true;
		}
		else if ((*it)->getType() == CAPTURE)
		{
			array.append(tmp->getX());
			array.append(tmp->getY());
			array.append(tmp->getDirX());
			array.append(tmp->getDirY());
			array.append(tmp->getWidth());
			array.append(tmp->getHeight());

			capture.append(array);
			bcapture = true;
		}
	}

	for (auto it = S_Map->getWalls()->begin(); it != S_Map->getWalls()->end(); ++it)
	{
		Json::Value array;
		AObject *tmp = (*it).get();

		array.append(tmp->getX());
		array.append(tmp->getY());
		array.append(tmp->getEndX());
		array.append(tmp->getEndY());

		wallsNoDir.append(array);
		bwalls = true;
	}

	if (brespawns)
		fromScratch["spawns"] = spawns;
	if (bflags)
		fromScratch["flags"] = flags;
	if (bwalls)
		fromScratch["walls_no_dir"] = wallsNoDir;
	if (bspeedFields)
		fromScratch["speed_fields"] = speed_fields;
	if (bcapture)
		fromScratch["capture"] = capture;

	Json::StyledWriter styledWriter;
	std::ofstream myfile;
	myfile.open(fullName.c_str(), std::ios::in | std::ios::trunc);
	myfile << styledWriter.write(fromScratch);
	myfile.close();
}