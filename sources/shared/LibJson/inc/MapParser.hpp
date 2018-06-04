//
// ConfigParser.hpp for pfa in /home/volent/dev/pfa/client/LibJson/src
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Wed Sep 11 16:37:04 2013 florent espanet
// Last update Sun Dec 15 23:36:44 2013 Gegon
//

#ifndef MAPPARSER_HPP_
# define MAPPARSER_HPP_

#include <iostream>
#include "json.hpp"
#include "Parser.hpp"
#include "Wall.hpp"

#include "MapStructures.h"

class MapParser : virtual public Parser
{
public:
  virtual void *parse();
};

#endif
