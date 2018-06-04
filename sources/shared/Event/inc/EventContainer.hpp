//
// EventContainer.hpp for Void Clash in /home/vial_j/travail/voidclash/client
// 
// Made by vial_j
// Login   <vial_j@epitech.net>
// 
// Started on  Thu Nov 14 00:41:28 2013 vial_j
// Last update Thu Nov 14 00:57:22 2013 vial_j
//

#ifndef		__EVENTCONTAINER_H__
# define	__EVENTCONTAINER_H__

#include	<list>
#include	<map>
#include	"EventComponent.hpp"

//////////////////////////////////////////////////////////////////////
/////	Class designed for stock sorted events list
//////////////////////////////////////////////////////////////////////

class		EventContainer
{
public:
  EventContainer(void);
  ~EventContainer(void);

  // Utility functions
  void clearEvents(void);
  void addEvent(eventType type, s_event event);

  // Getter
  std::list<std::pair<eventType, s_event> >* getEvents(void);

private:
  std::list<std::pair<eventType, s_event> > _events;
};

#endif		/* !__EVENTCONTAINER_H__ */
