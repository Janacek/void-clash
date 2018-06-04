//
// ConfigParser.hpp for pfa in /home/volent/dev/pfa/client/LibJson/src
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Wed Sep 11 16:37:04 2013 florent espanet
// Last update Wed Mar 19 11:38:05 2014 sadin_n
//

#ifndef PARSER_HPP_
# define PARSER_HPP_

#include <iostream>
#include "json.hpp"

class Parser
{
public:
  virtual ~Parser();

  virtual bool	loadFile(const char *path);
  virtual bool	loadFile(const std::string &path);
  virtual void	loadString(std::string input);
  virtual void	*parse() = 0;
  const std::string	&getString();

protected:
  std::string	_string;
  Json::Value	_root;
};

#endif
