/********************************************************************
**
**		Log.hpp
**		Created by : Vial Joris
**
********************************************************************/

#ifndef VOIDCLASH_LOG
#define VOIDCLASH_LOG

#include <SFML/System/Clock.hpp>
#include <fstream>
#include <string>
#include "SingletonBase.hpp"
#include "Defines.h"

/////////////////////////////////////////////////////////////////////
/////	Void clash client/server log system
/////////////////////////////////////////////////////////////////////

class CLog
{
public:
	CLog(void);
	~CLog(void);

	// Open the log file
	void start(unsigned int debugLevel, bool printMessagesInConsole = false);
	// Close and delete the log file if no errors occurs
	void stop(bool deleteLog = true);

	// Write an info depending if the debug level is >= 3
	void info(const std::string &message);
	// Write an info without checking the debug level
	void infoCritical(const std::string &message);

	// Write a warning if the debug level is >= 2
	void warning(const std::string &message);
	// Write a warning without checking the debug level
	void warningCritical(const std::string &message);

	// Write an error if the debug level is >= 1, throw is needThrow == true
	void error(const std::string &message, bool needThrow = false);
	// Write an error without checking the debug level, throw is needThrow == true
	void errorCritical(const std::string &message, bool needThrow = true);

private:
	unsigned int getCurrentTime(void);

private:
	bool _printMessagesInConsole;
	unsigned int _debugLevel;
	std::fstream _logFile;
	sf::Clock _currentTime;
	std::string _filename;
};

// Define singleton
typedef SingletonBase<CLog> LogSingleton;
#define S_Log LogSingleton::self()

// Shortcut
#define VC_INFO S_Log->info
#define VC_INFO_CRITICAL S_Log->infoCritical
#define VC_WARNING S_Log->warning
#define VC_WARNING_CRITICAL S_Log->warningCritical
#define VC_ERROR S_Log->error
#define VC_ERROR_CRITICAL S_Log->errorCritical

#endif /* VOIDCLASH_LOG */
