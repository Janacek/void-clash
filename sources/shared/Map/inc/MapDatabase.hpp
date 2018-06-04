#ifndef MapDatabase_HPP_
# define MapDatabase_HPP_

#include <deque>
#include <cstddef>
#include <string>

class MapDatabase
{
public:
	MapDatabase(const char *);
	~MapDatabase();

	const char *next();
	const char *prev();
	void load(const std::string &path);

	std::deque<std::string>	&getMapList();
	void addMap(const char *filename);
	void clearList();

	std::string &getCurrentMapName();
	void	setCurrentMap(char *mapName);
private:
	std::string _mapName;
	const char *_mapFolder;
	std::deque<std::string> _mapList;
	unsigned int _index;
};

#endif /* MapDatabase_HPP_ */
