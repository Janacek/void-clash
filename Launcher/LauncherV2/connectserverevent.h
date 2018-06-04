#ifndef CONNECTSERVEREVENT_H
#define CONNECTSERVEREVENT_H

#include <QEvent>

class ConnectServerEvent : public QEvent
{
public:
    ConnectServerEvent();
    ~ConnectServerEvent();

    static QEvent::Type type()
     {
        return (QEvent::Type)1003;
     }
};

#endif // ConnectSERVEREVENT_H
