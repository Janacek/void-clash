#include "connectserverevent.h"

ConnectServerEvent::ConnectServerEvent():
    QEvent(ConnectServerEvent::type())
{

}

ConnectServerEvent::~ConnectServerEvent()
{

}

