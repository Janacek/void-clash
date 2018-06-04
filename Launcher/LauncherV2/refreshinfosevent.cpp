#include "refreshinfosevent.h"

RefreshInfosEvent::RefreshInfosEvent(eInfoType type, eSelectionType info)
    : QEvent(RefreshInfosEvent::type()), _info(info), _type(type)
{
}

RefreshInfosEvent::~RefreshInfosEvent()
{

}

eSelectionType RefreshInfosEvent::getInfo() const
{
    return _info;
}

eInfoType RefreshInfosEvent::getType() const
{
    return _type;
}
