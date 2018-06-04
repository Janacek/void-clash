#ifndef CONFIGPARSER_HPP_
# define CONFIGPARSER_HPP_

#include <iostream>
#include "json.hpp"
#include "Parser.hpp"

typedef struct	s_server
{
	std::string name;
	std::string password;
	int			tickrate;
	int			max_player;
	int			min_player;
}		t_server;

typedef struct	s_game
{
	int		speed;
	float	zoom;
	int		friendly_fire_own;
	int		friendly_fire_team;
	int		round_nb;	// 0 = unlimited
	std::string	map;
	int			map_duration;
	std::string	mode;
	int		warmup_duration;

}		t_game;

typedef struct	s_player
{
  int		acceleration;
  int		max_speed;
  float		friction;
  int		size;
  float		velocity;
  int		max_energy;
  int		regen_energy;
  int		life;
  float		regen_life;
  int		speed_cap;
  float		invulnerable_time;	// in sec
}		t_player;

typedef struct	s_horde
{
	float	respawnTime;
	float	depopTime;
	int		speed;
	int		life;
	int		size;
	int		spawn_range;
	int		damage;
	int		acceleration;
}		t_horde;

typedef struct s_weapon
{
	int				getWeaponIndex();

	std::string		name;				// Name of the weapon
	std::string		category;			// Category of the weapon (ie. Bullet / Rocket...)
	int				energy_cost;		// All - Energy cose of the weapon
	int				init_energy_cost;	// Shield - Value of the drain energy when activating shield on the first frame
	int				damage;				// Bullet / Bomb / Rocket / Gravity - Damage
	int				speed;				// All - Velocity of the projectile
	float			duration;			// All - Duration of the projectile
	float			fire_rate;			// All - Firerate of the weapon
	int				size;				// All - Size of the projectile
	int				size_explosion;		// Rocket / Bomb - Size of the explosion
	int				pushback_fire;		// All - Pushback power on player when he fires
	int				pushback_other;		// All - Pushback power on other player when they get hit
	int				acceleration;		// Bomb / Shield - Give a speed boost
	int				chain;				// Bomb / Rocket - Is triggered when in range of another explosion
	int				drain_energy;		// All - Value of the drain energy when an enemy is hit
	int				shot_nb;			// Bullet - Nb of shots (ie. Shotgun)
	int				angle;				// Bullet - Spread angle of shots (ie. Shotgun)
	bool			collide_walls;		// All - true to enable collision with walls
	int				bounce;				// Bullet / Bomb - bounce agaisn't walls
	int				detection_range;	// Turret - Range of detection
	int				life;				// Turret - Turret life // 0 = immortal
	int				capacity;			// Turret / Gravity - Nb of elements
	int				slow;				// Bullet / Bomb / Rocket / Gravity - Power of slow in percent (0 - 100)
	float			slow_duration;		// Bullet / Bomb / Rocket - Duration of slow in sec

	struct s_weapon		*subWeapon;			// If a weapon has a subweapon (ie. Turret)
	std::string			subWeaponName;		// Just used for parsing

	// Displayed stats in the weapon selection
	std::string		desc;				// Description of the weapon
	std::vector<std::pair<std::string, int> >	*ratings;	// Ratings: name - value (between 0 and 10)
}	t_weapon;

typedef struct	s_config
{
  t_player	*player;
  t_horde	*horde;
  t_server	*server;
  t_game	*game;
  std::vector<t_weapon *>	*weapons;
}		t_config;

class ConfigParser : virtual public Parser
{
public:
  ConfigParser();
  virtual ~ConfigParser();

  virtual void *parse();

  t_config *loadDefaultConf();
};

#endif
