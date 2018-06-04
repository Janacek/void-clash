#include "GUIManager.hpp"

class NoLogger : public CEGUI::Logger
{
	void logEvent(const CEGUI::String&, CEGUI::LoggingLevel)
	{

	}
	void setLogFilename(const CEGUI::String&, bool)
	{

	}
};