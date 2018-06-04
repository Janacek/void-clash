//
// ConfigParser.cpp for pfa in /home/volent/dev/pfa/client/LibJson/src
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Wed Sep 11 16:42:30 2013 florent espanet
// Last update Wed Mar 19 11:30:37 2014 sadin_n
//

#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Parser.hpp"

Parser::~Parser()
{}

void	Parser::loadString(std::string input)
{
	_string = input;
}

const std::string	&Parser::getString()
{
	return _string;
}

bool Parser::loadFile(const char *path)
{
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (in)
    {
      in.seekg(0, std::ios::end);
      _string.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&_string[0], _string.size());
      in.close();
      return(true);
    }
  std::cerr << "Unable to load json file: " << path << std::endl;
  return (false);
}

bool Parser::loadFile(const std::string &path)
{
	std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		_string.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&_string[0], _string.size());
		in.close();
		return(true);
	}
	std::cerr << "Unable to load json file: " << path << std::endl;
	return (false);
}
