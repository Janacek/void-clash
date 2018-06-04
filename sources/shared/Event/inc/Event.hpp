//
// Event.hpp for Void Clash in /home/vial_j/travail/voidclash/client
// 
// Made by vial_j
// Login   <vial_j@epitech.net>
// 
// Started on  Thu Nov 14 00:41:12 2013 vial_j
// Last update Mon Jan 27 23:19:43 2014 sadin_n
//

#ifndef		__EVENTTYPE_H__
# define	__EVENTTYPE_H__

#include	<map>
#include	<list>
#include	"EventComponent.hpp"
#include	"EventContainer.hpp"


#if defined (_WIN32)
#define ADD_EVENT(X, Y) Event::addEvent(__FUNCTION__, __LINE__, X, Y)
#else
#define ADD_EVENT(X, Y) Event::addEvent(__PRETTY_FUNCTION__, __LINE__, X, Y)
#endif


#if defined (_WIN32)
#define ADD_EVENT_SIMPLE(X) Event::addEvent(__FUNCTION__, __LINE__, X)
#else
#define ADD_EVENT_SIMPLE(X) Event::addEvent(__PRETTY_FUNCTION__, __LINE__, X)
#endif


namespace	Event
{
  class		EventUtils
  {
  public:
    EventUtils(void);
    ~EventUtils(void);

    static EventUtils* getInstance(void);
  
    void clearEvents(void);
	void addEvent(const std::string& funcName, int line, eventType type, s_event event);
    bool eventAlreadyAddedForId(eventType type, sf::Uint32 id);
	bool eventAlreadyAddedForEntity(eventType type, const std::shared_ptr<AObject> &entity);
    std::list<std::pair<eventType, s_event> >* getEventByType(eventType type);
	std::list<std::pair<eventType, s_event> >* getEventByObject(const std::shared_ptr<AObject> &trigger);
    std::list<std::pair<eventType, s_event> >* getMainEventList(void);

  private:
    static EventUtils *_instance;
	std::map<std::shared_ptr<AObject>, EventContainer> _eventByObject;
    std::map<eventType, EventContainer> _eventByType;
    std::list<std::pair<eventType, s_event> >_main;
  };

  void clearEvents(void);
  void addEvent(const std::string &funcName, int line, eventType type, s_event event = s_event());
  bool eventAlreadyAddedForId(eventType type, sf::Uint32 id);
  bool eventAlreadyAddedForEntity(eventType type, const std::shared_ptr<AObject> &entity);
  std::list<std::pair<eventType, s_event> >* getEventByType(eventType type);
  std::list<std::pair<eventType, s_event> >* getEventByObject(const std::shared_ptr<AObject> &trigger);
  std::list<std::pair<eventType, s_event> >* getMainEventList(void);
}

#endif		/* !__EVENTTYPE_H__ */
