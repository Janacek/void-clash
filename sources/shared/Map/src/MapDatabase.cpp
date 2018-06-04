#include "Files.hpp"
#include "MapDatabase.hpp"
#include "AssetPath.h"

extern std::string G_configPath;

MapDatabase::MapDatabase(const char *mapFolder)
  : _mapFolder(mapFolder)
{
	load(G_configPath + _mapFolder);
  _index = 0;

}

MapDatabase::~MapDatabase()
{}

const char	*MapDatabase::next()
{
  if (_mapList.size() == 0)
    return (NULL);
  if (_index >= _mapList.size() - 1)
    _index = 0;
  else
    ++_index;
  _mapName = (char *)_mapList[_index].c_str();
  return (_mapList[_index].c_str());
}

const char	*MapDatabase::prev()
{
  if (_mapList.size() == 0)
    return (NULL);
  if (_index <= 0)
    _index = _mapList.size() - 1;
  else
    --_index;
  _mapName = (char *)_mapList[_index].c_str();
  return (_mapList[_index].c_str());
}

void		MapDatabase::load(const std::string &path)
{
  void		*handle = NULL;

  _mapFolder = path.c_str();
  char *mapName;
  if (!(handle = Files::first(_mapFolder, &mapName)))
    return ;
  if (Files::isFile(Files::getPath(_mapFolder, mapName)))
	  _mapList.push_back(mapName);
  while ((mapName = Files::next(handle)))
	  if (Files::isFile(Files::getPath(_mapFolder, mapName)))
		  _mapList.push_back(mapName);
  _mapName = (char *)_mapList.front().c_str();
}

std::deque<std::string>	&MapDatabase::getMapList()
{
	return _mapList;
}

void	MapDatabase::addMap(const char *filename)
{
	_mapList.push_back(filename);
}

void	MapDatabase::clearList()
{
	_mapName.clear();
	_mapList.clear();
}

//------------------------------------------------------------------//
// Current Map

void	MapDatabase::setCurrentMap(char *mapName)
{
	_mapName = mapName;
}

std::string &MapDatabase::getCurrentMapName()
{
	return _mapName;
}