#ifndef SELECTSERVEREVENT_H
#define SELECTSERVEREVENT_H

#include <QEvent>

class SelectServerEvent : public QEvent
{
public:
    SelectServerEvent();
    ~SelectServerEvent();

    static QEvent::Type type()
     {
        return (QEvent::Type)1002;
     }
};

#endif // SELECTSERVEREVENT_H
