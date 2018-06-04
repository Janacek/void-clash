#ifndef MAPSAVER_H_
#define MAPSAVER_H_

#include <iostream>
#include "json.hpp"
#include "Parser.hpp"
#include "Wall.hpp"

#include "MapStructures.h"

class MapSaver
{
public:
	virtual void saveMap(t_map *map, const std::string &, const std::string &);
};

#endif