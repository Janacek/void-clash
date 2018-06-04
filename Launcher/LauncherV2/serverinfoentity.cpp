#include "serverinfoentity.h"
#include <iostream>

ServerInfoEntity::ServerInfoEntity(ServerListView *sw, std::string name, std::string mode,
                                   std::string playerNb, std::string map, std::string ip,
                                   std::string port, QWidget *parent)
    : QWidget(parent), _sw(sw), _name(name), _mode(mode), _map(map), _playerNb(playerNb), _ip(ip), _port(port), _selected(false)
{
}

ServerInfoEntity::ServerInfoEntity(ServerListView *sw, ServerInfoEntity *copy):
    QWidget(NULL), _sw(sw), _selected(false)
{
    _name = copy->getName();
    _mode = copy->getMode();
    _map = copy->getMap();
    _playerNb = copy->getPlayerNb();
    _ip = copy->getIp();
}

ServerInfoEntity::~ServerInfoEntity()
{
    delete _nameLbl;
    delete _modeLbl;
    delete _mapLbl;
    delete _playerNbLbl;
}

void    ServerInfoEntity::init()
{
    setFixedSize(590, 60);
    setStyleSheet("background-color:rgb(120, 120, 120, 80)");
    QFont bold("Gotham Bold");
    bold.setPointSize(8);

    if (_name.size() > 21) {
        _name.resize(21);
        _name += "..";
    }
    _nameLbl = new QLabel(_name.c_str(), this);
    _nameLbl->setFont(bold);
    _nameLbl->setStyleSheet("color : rgb(206, 207, 212);"
                            "background-color:rgb(40, 40, 40, 0);");
    _nameLbl->move(35, 25);


    bold.setPointSize(9);
    _modeLbl = new QLabel(_mode.c_str(), this);
    _modeLbl->setFont(bold);
    _modeLbl->setStyleSheet("color : rgb(206, 207, 212);"
                            "background-color:rgb(40, 40, 40, 0);");
    _modeLbl->move(195, 25);

    _playerNbLbl = new QLabel(_playerNb.c_str(), this);
    _playerNbLbl->setFont(bold);
    _playerNbLbl->setStyleSheet("color : rgb(206, 207, 212);"
                            "background-color:rgb(40, 40, 40, 0);");
    _playerNbLbl->move(380, 25);

    _mapLbl = new QLabel(_map.c_str(), this);
    _mapLbl->setFont(bold);
    _mapLbl->setStyleSheet("color : rgb(206, 207, 212);"
                            "background-color:rgb(40, 40, 40, 0);");
    _mapLbl->move(485, 25);
}

bool ServerInfoEntity::isSelected()
{
    return _selected;
}

void ServerInfoEntity::setSelected(bool selected)
{
    if (!selected)
        setStyleSheet("background-color:rgb(120, 120, 120, 80)");
    else
        setStyleSheet("background-color:rgb(255, 255, 255, 150)");
    _selected = selected;
}

void ServerInfoEntity::enterEvent( QEvent* e )
{
    if (!_selected)
        setStyleSheet("background-color:rgb(160, 160, 160, 80)");
    QWidget::enterEvent( e );
}

void ServerInfoEntity::leaveEvent( QEvent* e )
{
    if (!_selected)
        setStyleSheet("background-color:rgb(120, 120, 120, 80)");
    QWidget::leaveEvent( e );
}

void ServerInfoEntity::mouseReleaseEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton) {
        QCoreApplication::sendEvent((QObject*)_sw, new SelectServerEvent());
        setSelected(true);
    }
}

void ServerInfoEntity::mouseDoubleClickEvent( QMouseEvent * e )
{
    if ( e->button() == Qt::LeftButton )
    {
        QCoreApplication::sendEvent((QObject*)_sw, new ConnectServerEvent());
    }
}

std::string ServerInfoEntity::getName()
{
    return _name;
}

std::string ServerInfoEntity::getMode()
{
    return _mode;
}

std::string ServerInfoEntity::getMap()
{
    return _map;
}

std::string ServerInfoEntity::getPlayerNb()
{
    return _playerNb;
}

std::string ServerInfoEntity::getIp()
{
    return _ip;
}

std::string ServerInfoEntity::getPort()
{
    return _port;
}

std::string ServerInfoEntity::getInfoByType(eSortType type)
{
    if (type == NAMESORT)
        return _name;
    else if (type == MODESORT)
         return _mode;
    else if (type == MAPSORT)
        return _map;
    else if (type == PLAYERNBSORT)
        return _playerNb;
    return std::string("");
}

//----------------
// PRIVATE FUNCTIONS
//----------------
void    ServerInfoEntity::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

//----------------
// PUBLIC SLOTS
//----------------
void    ServerInfoEntity::__switchSelection()
{
    _selected ^= true;
    setSelected(_selected);
}
