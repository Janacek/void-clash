//
// ConfigParser.cpp for pfa in /home/volent/dev/pfa/client/LibJson/src
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Wed Sep 11 16:42:30 2013 florent espanet
// Last update Fri Jan 24 11:39:21 2014 florent espanet
//

#include <iostream>
#include <fstream>
#include "json.hpp"
#include  "Wall.hpp"
#include "MapParser.hpp"

void *MapParser::parse()
{
  Json::Reader	reader;
  t_map		*conf;
  unsigned int zero = 0;
  
  if (!reader.parse(_string.c_str(), _root, false))
    {
      std::cerr << "Error :" << reader.getFormatedErrorMessages() << std::endl;
      return (NULL);
    }

  float ratio			= _root["scale"].asFloat();
  const Json::Value flags	= _root["flags"];
  const Json::Value n_walls	= _root["walls_no_dir"];
  const Json::Value v_walls	= _root["vertical_walls"];
  const Json::Value h_walls	= _root["horizontal_walls"];
  const Json::Value g_fields	= _root["gravity_fields"];
  const Json::Value capture = _root["capture"];
  const Json::Value s_fields	= _root["speed_fields"];
  const Json::Value spawns	= _root["spawns"];

  if (ratio == 0)
    ratio = 1;
  conf		= new t_map;
  conf->flags = new t_flags[flags.size()];
  conf->wallsNoDir = new t_wallsNoDir[n_walls.size()];
  conf->walls	= new t_walls[v_walls.size() + h_walls.size()];
  conf->grav	= new t_grav[g_fields.size()];
  conf->capture = new t_capture[capture.size()];
  conf->speed	= new t_speed[s_fields.size()];
  conf->spawn  = new t_spawn[spawns.size()];

  conf->nflags	= flags.size();
  conf->nwallNoDir	= n_walls.size();
  conf->nwall	= v_walls.size() + h_walls.size();
  conf->ngrav	= g_fields.size();
  conf->nspeed	= s_fields.size();
  conf->nspawn	= spawns.size();
  conf->ncapture = capture.size();

  int nb = 0;


  for (unsigned int i = 0; i < s_fields.size(); ++i)
  {
	  if (s_fields[i].size() != 6)
	  {
		  std::cerr << "Error : speed field definition should contain six parameters." << std::endl;
		  conf->speed[i].error = true;
		  continue;
	  }
	  conf->speed[i].X = s_fields[i][zero].asFloat() * ratio;
	  conf->speed[i].Y = s_fields[i][1].asFloat() * ratio;
	  conf->speed[i].dirX = s_fields[i][2].asFloat() * ratio;
	  conf->speed[i].dirY = s_fields[i][3].asFloat() * ratio;
	  conf->speed[i].width = s_fields[i][4].asFloat() * ratio;
	  conf->speed[i].height = s_fields[i][5].asFloat() * ratio;
	  conf->speed[i].error = false;

	  conf->speed[i].id = nb;
	  ++nb;
  }

  for (unsigned int i = 0; i < n_walls.size(); ++i)
    {
      if (n_walls[i].size() != 4)
	{
	  std::cerr << "Error : wall definition should contain four parameters." << std::endl;
	  conf->walls[i].error = true;
	  continue ;
	}
      conf->wallsNoDir[i].X = n_walls[i][zero].asFloat() * ratio;
      conf->wallsNoDir[i].Y = n_walls[i][1].asFloat() * ratio;
      conf->wallsNoDir[i].endX = n_walls[i][2].asFloat() * ratio;
      conf->wallsNoDir[i].endY = n_walls[i][3].asFloat() * ratio;
      conf->wallsNoDir[i].error = false;

	  conf->wallsNoDir[i].id = nb;
	  ++nb;
    }


  for (unsigned int i = 0; i < v_walls.size(); ++i)
    {
      if (v_walls[i].size() != 3)
	{
	  std::cerr << "Error : wall definition should contain three parameters." << std::endl;
	  conf->walls[i].error = true;
	  continue ;
	}
      conf->walls[i].X = v_walls[i][zero].asFloat() * ratio;
      conf->walls[i].Y = v_walls[i][1].asFloat() * ratio;
      conf->walls[i].len = v_walls[i][2].asInt() * ratio;
      conf->walls[i].orientation = VERTICAL;
      conf->walls[i].error = false;

	  conf->walls[i].id = nb;
	  ++nb;
  }



  for (unsigned int i = 0; i < h_walls.size(); ++i)
    {
      if (h_walls[i].size() != 3)
	{
	  std::cerr << "Error : wall definition should contain three parameters." << std::endl;
	  conf->walls[i + v_walls.size()].error = true;
	  continue ;
	}
      conf->walls[i + v_walls.size()].X = h_walls[i][zero].asFloat()* ratio;
      conf->walls[i + v_walls.size()].Y = h_walls[i][1].asFloat() * ratio;
      conf->walls[i + v_walls.size()].len = h_walls[i][2].asInt() * ratio;
      conf->walls[i + v_walls.size()].orientation = HORIZONTAL;
      conf->walls[i + v_walls.size()].error = false;

	  conf->walls[i].id = nb;
	  ++nb;
  }
 // for (unsigned int i = 0; i < g_fields.size(); ++i)
 //   {
 //     if (g_fields[i].size() != 6)
	//{
	//  std::cerr << "Error : gravity field definition should contain six parameters." << std::endl;
	//  conf->grav[i].error = true;
	//  continue ;
	//}
 //     conf->grav[i].X = g_fields[i][zero].asFloat() * ratio;
 //     conf->grav[i].Y = g_fields[i][1].asFloat() * ratio;
 //     conf->grav[i].dirX = g_fields[i][2].asFloat() * ratio;
 //     conf->grav[i].dirY = g_fields[i][3].asFloat() * ratio;
 //     conf->grav[i].size = g_fields[i][4].asInt() * ratio;
 //     conf->grav[i].power = g_fields[i][5].asFloat() * ratio;
 //     conf->grav[i].error = false;

	//  conf->grav[i].id = nb;
	//  ++nb;
 // }

  for (unsigned int i = 0; i < spawns.size(); ++i)
  {
	  if (spawns[i].size() != 7)
	  {
		  std::cerr << "Error : spawn definition should contain seven parameters." << std::endl;
		  conf->spawn[i].error = true;
		  continue;
	  }
	  conf->spawn[i].X = spawns[i][zero].asFloat() * ratio;
	  conf->spawn[i].Y = spawns[i][1].asFloat() * ratio;
	  conf->spawn[i].dirX = spawns[i][2].asFloat() * ratio;
	  conf->spawn[i].dirY = spawns[i][3].asFloat() * ratio;
	  conf->spawn[i].width = spawns[i][4].asFloat() * ratio;
	  conf->spawn[i].height = spawns[i][5].asFloat() * ratio;
	  conf->spawn[i].team = spawns[i][6].asInt();
	  conf->spawn[i].error = false;

	  conf->spawn[i].id = nb;
	  ++nb;
  }



  for (unsigned int i = 0; i < flags.size(); ++i)
    {
      if (flags[i].size() != 5)
	{
	  std::cerr << "Error : flags definition should contain five parameters." << std::endl;
	  conf->flags[i].error = true;	  
	  continue ;
	}
      conf->flags[i].X = flags[i][zero].asFloat() * ratio;
      conf->flags[i].Y = flags[i][1].asFloat() * ratio;
      conf->flags[i].dirX = flags[i][2].asFloat() * ratio;
      conf->flags[i].dirY = flags[i][3].asFloat() * ratio;
      conf->flags[i].team = flags[i][4].asInt();
      conf->flags[i].error = false;

	  conf->flags[i].id = nb;
	  ++nb;
  }

  for (unsigned int i = 0; i < capture.size(); ++i)
  {
	  if (capture[i].size() != 6)
	  {
		  std::cerr << "Error : capture definition should contain six parameters." << std::endl;
		  conf->capture[i].error = true;
		  continue;
	  }
	  conf->capture[i].X = capture[i][zero].asFloat() * ratio;
	  conf->capture[i].Y = capture[i][1].asFloat() * ratio;
	  conf->capture[i].dirX = capture[i][2].asFloat() * ratio;
	  conf->capture[i].dirY = capture[i][3].asFloat() * ratio;
	  conf->capture[i].width = capture[i][4].asFloat() * ratio;
	  conf->capture[i].height = capture[i][5].asFloat() * ratio;
	  conf->capture[i].error = false;

	  conf->capture[i].id = nb;
	  ++nb;
  }

  return ((void *)conf);
}
