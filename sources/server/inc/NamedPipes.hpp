//
// NamedPipes.hpp for NamedPipes in /home/sadin_n/pfa
// 
// Made by sadin_n
// Login   <sadin_n@epitech.net>
// 
// Started on  Sat Jan 18 00:30:58 2014 sadin_n
// Last update Sat Jan 18 01:44:03 2014 sadin_n
//

#ifndef		NAMEDPIPES_HPP_
# define	NAMEDPIPES_HPP_

#include	<iostream>

class	       INamedPipes
{
public:
  virtual ~INamedPipes() {};

  virtual int	initFifo() = 0;
  virtual int	prepareMsg(bool) = 0;
  virtual int	removeFifo(const std::string &) = 0;
  virtual int	sendMsg(const std::string &) = 0;
  virtual const std::string rcvMsg() = 0;
};

#if defined(linux) || defined(linux)

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

class		NamedPipesL : public INamedPipes
{
public:
  NamedPipesL(): _fifoFile(".serverFifo") {};
  virtual ~NamedPipesL()
  {
    unlink(_fifoFile.c_str());
  };

  virtual int	initFifo()
  {
    if (mkfifo(_fifoFile.c_str(), S_IRWXU) == -1)
      return removeFifo(std::string("Launcher : cannot create " + _fifoFile));
    return 0;
  };

  virtual int	removeFifo(const std::string &str)
  {
    unlink(_fifoFile.c_str());
    std::cerr << str << std::endl;
    return -1;
  }

  virtual int	prepareMsg(bool write)
  {
    if (write)
      _fileOut.open(_fifoFile.c_str(), std::fstream::out);
    else
      _fileIn.open(_fifoFile.c_str(), std::fstream::in);
    if (write ? _fileOut.is_open() : _fileIn.is_open())
      {
	std::cerr << "Server : Cannot open " << _fifoFile << std::endl;
	return removeFifo(std::string("Launcher : cannot open " + _fifoFile));
      }
    return 0;
  }

  virtual int	sendMsg(const std::string &str)
  {
    _fileOut << str;
    if (_fileOut.fail() || _fileOut.bad())
      return removeFifo(std::string("Launcher : cannot write on " + _fifoFile));
    _fileOut.close();
    return 0;
  };

  virtual const std::string rcvMsg()
  {
    std::string	str = "";

    _fileIn.read((char *)str.c_str(), sizeof(str));
    if (_fileIn.fail() || _fileIn.bad() || _fileIn.eof())
      {
	removeFifo(std::string("Launcher : cannot read on " + _fifoFile));
	return str;
      }
    _fileIn.close();
    return str;
  };

private:
  std::string _fifoFile;
  std::ofstream _fileOut;
  std::ifstream _fileIn;
};

#elif defined(_WIN32) || defined(__WIN32__)
class		NamedPipesW : public INamedPipes
{
  NamedPipesW() {};
  virtual ~NamedPipesW() {};

  virtual int	initFifo() {};
  virtual int	prepareMsg(bool) {};
  virtual int	removeFifo(const std::string &str) {};
  virtual int	sendMsg(const std::string &str) {};
  virtual const std::string rcvMsg() {};
};
#endif

#endif		/* NAMEDPIPES_HPP_ */
