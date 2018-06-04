/********************************************************************
**
**		Log.cpp
**		Created by : Vial Joris
**
********************************************************************/

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "Log.hpp"

extern bool G_isServer;

/////////////////////////////////////////////////////////////////////
/////	Constructor / Destructor
/////////////////////////////////////////////////////////////////////

CLog::CLog(void)
{
	_debugLevel = DEBUG_LEVEL;
	_printMessagesInConsole = false;
	_filename = "";
}

CLog::~CLog(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Open the log file
////	Warning: If printMessageInConsole is set to true
////	You will experience lags and fps drop due to spam
/////////////////////////////////////////////////////////////////////

void CLog::start(unsigned int debugLevel, bool printMessagesInConsole)
{
	if (_debugLevel == 0)
		return;

	_debugLevel = debugLevel;
	_printMessagesInConsole = printMessagesInConsole;

	if (_logFile.is_open() == false)
	{
		if (G_isServer == true)
			_filename = "serverLog";
		else
			_filename = "clientLog";

		_logFile.open(_filename, std::fstream::out);
		_currentTime.restart();
	}
}

/////////////////////////////////////////////////////////////////////
/////	Close the log file if no errors occurs
/////////////////////////////////////////////////////////////////////

void CLog::stop(bool deleteLog)
{
	if (_logFile.is_open() == true)
	{
		_logFile.close();

		// If the game close normally, delete it
		if (deleteLog)
			std::remove(_filename.c_str());
		// Else rename it with the current date/time
		else
		{
			#ifdef _WIN32
			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);
			std::stringstream ss("");
			ss << _filename;
			ss << std::put_time(&tm, "_%d-%m-%Y_%H-%M-%S.log");
			std::rename(_filename.c_str(), ss.str().c_str());
			#endif
		}
	}
}

/////////////////////////////////////////////////////////////////////
/////	Info
/////	_debugLevel >= 3
/////////////////////////////////////////////////////////////////////

void CLog::info(const std::string &message)
{
	if (_debugLevel >= 3)
	{
		if (_logFile.is_open() == false)
			return;

		_logFile << "[" << getCurrentTime() << "] INFO: " << message << std::endl;

		if (_printMessagesInConsole == true)
			std::cout << "[" << getCurrentTime() << "] INFO: " << message << std::endl;
	}
}

void CLog::infoCritical(const std::string &message)
{
	if (_logFile.is_open() == false)
		return;

	std::cout << "[" << getCurrentTime() << "] INFO: " << message << std::endl;
	_logFile << "[" << getCurrentTime() << "] INFO: " << message << std::endl;
}

/////////////////////////////////////////////////////////////////////
/////	Warning
////	_debugLevel >= 2
/////////////////////////////////////////////////////////////////////

void CLog::warning(const std::string &message)
{
	if (_debugLevel >= 2)
	{
		if (_logFile.is_open() == false)
			return;

		_logFile << "[" << getCurrentTime() << "] WARNING: " << message << std::endl;
		if (_printMessagesInConsole == true)
			std::cout << "[" << getCurrentTime() << "] WARNING: " << message << std::endl;
	}
}

void CLog::warningCritical(const std::string &message)
{
	if (_logFile.is_open() == false)
		return;

	std::cout << "[" << getCurrentTime() << "] WARNING: " << message << std::endl;
	_logFile << "[" << getCurrentTime() << "] WARNING: " << message << std::endl;
}

/////////////////////////////////////////////////////////////////////
/////	Error
////	_debugLevel >= 1
/////////////////////////////////////////////////////////////////////

void CLog::error(const std::string &message, bool needThrow)
{
	if (_debugLevel >= 1)
	{
		if (_logFile.is_open() == false)
			return;

		_logFile << "[" << getCurrentTime() << "] ERROR: " << message << std::endl;
		if (_printMessagesInConsole == true)
			std::cout << "[" << getCurrentTime() << "] ERROR: " << message << std::endl;

		// Close the log file
		if (needThrow == true)
		{
			stop();
			// Throw an exception
			throw std::runtime_error(message);
		}
	}
}

void CLog::errorCritical(const std::string &message, bool needThrow)
{
	if (_logFile.is_open() == false)
		return;

	_logFile << "[" << getCurrentTime() << "] ERROR: " << message << std::endl;
	std::cout << "[" << getCurrentTime() << "] ERROR: " << message << std::endl;

	// Close the log file
	if (needThrow == true)
	{
		stop();
		// Throw an exception
		throw std::runtime_error(message);
	}
}

/////////////////////////////////////////////////////////////////////
/////	Return current time in seconds
/////////////////////////////////////////////////////////////////////

unsigned int CLog::getCurrentTime(void)
{
	return _currentTime.getElapsedTime().asSeconds();
}
