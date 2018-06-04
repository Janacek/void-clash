#ifndef MAPSTRUCTURES_H_
#define MAPSTRUCTURES_H_

#include "Wall.hpp"

typedef struct s_map_obj
{

}		t_map_obj;

typedef struct	s_flags : public t_map_obj
{
	float		X;
	float		Y;
	float		dirX;
	float		dirY;
	float		endX;
	float		endY;
	float		width;
	float		height;
	int			team;
	float		power;
	int			size;
	bool		error;

	int			id;
}		t_flags;

typedef struct	s_wallsNoDir : public t_map_obj
{
	float		X;
	float		Y;
	float		dirX;
	float		dirY;
	float		endX;
	float		endY;
	float		width;
	float		height;
	int			team;
	float		power;
	int			size;
	bool		error;

	int			id;
}		t_wallsNoDir;

typedef struct	s_walls : public t_map_obj
{
	float		X;
	float		Y;
	int		len;
	eOrientation orientation;
	bool		error;

	int			id;
}		t_walls;

typedef struct	s_grav : public t_map_obj
{
	float		X;
	float		Y;
	float		dirX;
	float		dirY;
	float		endX;
	float		endY;
	float		width;
	float		height;
	int			team;
	float		power;
	int			size;
	bool		error;

	int			id;
}		t_grav;

typedef struct	s_capture : public t_map_obj
{
	float		X;
	float		Y;
	float		dirX;
	float		dirY;
	float		endX;
	float		endY;
	float		width;
	float		height;
	int			team;
	float		power;
	int			size;
	bool		error;

	int			id;
}		t_capture;

typedef struct	s_speed : public t_map_obj
{
	float		X;
	float		Y;
	float		dirX;
	float		dirY;
	float		endX;
	float		endY;
	float		width;
	float		height;
	int			team;
	float		power;
	int			size;
	bool		error;

	int			id;
}		t_speed;

typedef struct	s_spawn : public t_map_obj
{
	float		X;
	float		Y;
	float		dirX;
	float		dirY;
	float		endX;
	float		endY;
	float		width;
	float		height;
	int			team;
	float		power;
	int			size;
	bool		error;

	int			id;
}		t_spawn;



typedef struct	s_map
{
	t_flags	*flags;
	int		nflags;
	t_wallsNoDir	*wallsNoDir;
	int		nwallNoDir;
	t_walls	*walls;
	int		nwall;
	t_grav	*grav;
	int		ngrav;
	t_speed	*speed;
	int		nspeed;
	t_spawn	*spawn;
	int		nspawn;
	t_capture	*capture;
	int		ncapture;
}		t_map;

#endif