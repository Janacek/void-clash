//////////////////////////////////////////////////////
// Event.cpp for project Void Clash
// 
// Made by Vial Joris
// 
// Started on  Tue Nov 12 20:01:01 2013 Vial Joris
// Updated on  Wed Nov 13 21:18:34 2013 Vial Joris
//////////////////////////////////////////////////////

#include	"Log.hpp"
#include "Event.hpp"

//////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
//////////////////////////////////////////////////////////////////////

using namespace Event;

EventUtils *EventUtils::_instance = NULL;

EventUtils::EventUtils(void)
{
	_main.clear();
	_eventByObject.clear();
	_eventByType.clear();
}

EventUtils::~EventUtils(void)
{
}

EventUtils* EventUtils::getInstance(void)
{
	if (EventUtils::_instance == NULL)
		EventUtils::_instance = new EventUtils;
	return _instance;
}

//////////////////////////////////////////////////////////////////////
/////	Utility functions
//////////////////////////////////////////////////////////////////////

void	EventUtils::clearEvents(void)
{
	{
		std::map<eventType, EventContainer>::iterator it;
		for (it = _eventByType.begin(); it != _eventByType.end(); ++it)
			it->second.clearEvents();
	}

	std::map<std::shared_ptr<AObject>, EventContainer>::iterator it;
	for (it = _eventByObject.begin(); it != _eventByObject.end(); ++it)
		it->second.clearEvents();

	_main.clear();
	_eventByObject.clear();
	_eventByType.clear();
}

void	EventUtils::addEvent(const std::string& funcName, int line, eventType type, s_event event)
{
	event.raiseInfo = "Event raised in " + funcName + " at line " + std::to_string(line);

	// If event is not defined, ignore
	if (type == ev_UNKNOWN)
		return;

	if (!event.trigger && type == ev_DELETE)
		VC_WARNING_CRITICAL("Event delete with nullptr detected");

	if (type == ev_DELETE && eventAlreadyAddedForEntity(ev_DELETE, event.trigger) == true)
		return;

	// Add events in main container
	_main.push_back(std::pair<eventType, s_event>(type, event));

	// Add events in sorted containers
	_eventByType[type].addEvent(type, event);
	if (event.trigger != NULL)
		_eventByObject[event.trigger].addEvent(type, event);
}

bool  EventUtils::eventAlreadyAddedForId(eventType type, sf::Uint32 id)
{
	if (Event::getEventByType(type))
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(type)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(type)->end();

		while (it != end)
		{
			// Il sert à rien ce test ???
			if (it->first == type)
			{
				if ((((it->second.trigger))->getId()) == id)
					return true;
			}
			++it;
		}
	}
	return false;
}

bool  EventUtils::eventAlreadyAddedForEntity(eventType type, const std::shared_ptr<AObject> &entity)
{
	if (Event::getEventByType(type))
	{
		std::list<std::pair<eventType, s_event> >::const_iterator it = Event::getEventByType(type)->begin();
		std::list<std::pair<eventType, s_event> >::const_iterator end = Event::getEventByType(type)->end();

		while (it != end)
		{
			if (it->second.trigger == entity)
				return true;
			++it;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
/////	Getters methods, !! return NULL if entry doesn't matches !!
//////////////////////////////////////////////////////////////////////

std::list<std::pair<eventType, s_event> >* EventUtils::getMainEventList(void)
{
	return &_main;
}

//------------------------------------------------------------------//

std::list<std::pair<eventType, s_event> >* EventUtils::getEventByType(eventType type)
{
	std::map<eventType, EventContainer>::const_iterator found = _eventByType.find(type);

	if (found != _eventByType.end())
		return _eventByType[type].getEvents();
	return NULL;
}

//------------------------------------------------------------------//

std::list<std::pair<eventType, s_event> >* EventUtils::getEventByObject(const std::shared_ptr<AObject> &trigger)
{
	std::map<std::shared_ptr<AObject>, EventContainer>::const_iterator found = _eventByObject.find(trigger);

	if (found != _eventByObject.end())
		return _eventByObject[trigger].getEvents();
	return NULL;
}

//////////////////////////////////////////////////////////////////////
/////	Namespace functions
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//

void Event::clearEvents(void)
{
	EventUtils::getInstance()->clearEvents();
}

//------------------------------------------------------------------//

void Event::addEvent(const std::string &funcName, int line, eventType type, s_event event)
{
	return EventUtils::getInstance()->addEvent(funcName, line, type, event);
}

//------------------------------------------------------------------//

std::list<std::pair<eventType, s_event> >* Event::getEventByType(eventType type)
{
	return EventUtils::getInstance()->getEventByType(type);
}

//------------------------------------------------------------------//
std::list<std::pair<eventType, s_event> >* Event::getEventByObject(const std::shared_ptr<AObject> &trigger)
{
	return EventUtils::getInstance()->getEventByObject(trigger);
}

//------------------------------------------------------------------//
std::list<std::pair<eventType, s_event> >* Event::getMainEventList(void)
{
	return EventUtils::getInstance()->getMainEventList();
}

//------------------------------------------------------------------//
bool  Event::eventAlreadyAddedForId(eventType type, sf::Uint32 id)
{
	return EventUtils::getInstance()->eventAlreadyAddedForId(type, id);
}

//------------------------------------------------------------------//
bool  Event::eventAlreadyAddedForEntity(eventType type, const std::shared_ptr<AObject>&entity)
{
	return EventUtils::getInstance()->eventAlreadyAddedForEntity(type, entity);
}
