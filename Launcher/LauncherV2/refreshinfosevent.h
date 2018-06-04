#ifndef REFRESHINFOSEVENT_H
#define REFRESHINFOSEVENT_H

#include <QEvent>
#include <iostream>

#include "common.h"

class RefreshInfosEvent : public QEvent
{
public:
    RefreshInfosEvent(eInfoType type, eSelectionType selection);
    ~RefreshInfosEvent();

    eSelectionType getInfo() const;
    eInfoType   getType() const;

    static QEvent::Type type()
     {
        return (QEvent::Type)1001;
     }

private:
    eSelectionType _info;
    eInfoType   _type;
};

#endif // REFRESHINFOSEVENT_H
