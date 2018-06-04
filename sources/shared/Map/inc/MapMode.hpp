#ifndef		MAPMODE_HPP_
# define	MAPMODE_HPP_

#include <SFML/System.hpp>

enum		eMapMode
{
	mode_NONE,
	mode_FFA,
	mode_TEAM_DM,
	mode_CTF,
	mode_SURVIVOR,
	mode_TEAM_SURVIVOR,
	mode_CAPTURE
};

static const char * eMapModeString[] = {
	"None",
	"Free For All",
	"Team Deathmatch",
	"Capture The Flag",
	"Survivor",
	"Team Survivor",
	"Capture",
	NULL
};

#define END_ROUND_DURATION			5		// in sec - used to display round winner

// Defines the property of the mode
typedef struct s_mode
{
	bool	team;				// Is team based
	bool	flag;				// Flag active
	int		respawnTime;		// -1 to use round mode (survivor style)
	bool	capture;				// Use capture zone (king of the hill style)
	bool	horde;				// Bots appears

	bool	killIncTeamScore;	// When a player from a team kill another player, inc score team
}	t_mode;

class	Bot;

// Handle game mode and rounds
class MapMode
{
public:
	MapMode();
	~MapMode();

	bool	update();

	void	initFromConfig();
	void	init();

	void	activeHorde();
	void	desactiveHorde();
	bool	isHordeActivated();

	void		changeMapMode(eMapMode mode);
	eMapMode	getModeEnum();
	eMapMode	findModeFromString(std::string &mode);
	int			getRoundNumber();

	void		changeRound();
	void		setRoundNb(int roundNb);

	static const char *getModeNameById(int id)
	{
		return eMapModeString[id];
	};

	s_mode				*getProperty();
	const std::string	&getModeName();
private:
	eMapMode			_mode;
	std::string			_modeName;
	s_mode				*_property;

	int					_roundNb;

	sf::Clock			_clock;
	bool				_endRound;
	sf::Time			_endOfRoundTime;

	sf::Time			_lastRespawnHorde;

	// Funcs
	void	handleRounds();
	bool	checkRoundVictory();

	void	handleHorde();
	void	spawnBot();
	bool	entityInSpawnRange(Bot *bot);

	void		clearRound();
};

#endif
