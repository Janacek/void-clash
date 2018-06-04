#ifndef SERVERINFOENTITY_H
#define SERVERINFOENTITY_H

#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include <QLabel>
#include <QFont>
#include <QCoreApplication>
#include "selectserverevent.h"
#include "connectserverevent.h"

enum eSortType {
    NAMESORT = 0,
    MODESORT = 1,
    MAPSORT = 2,
    PLAYERNBSORT = 3,
    NONESORT = 4
};

class ServerListView;

class ServerInfoEntity: public QWidget
{
    Q_OBJECT
public:
    ServerInfoEntity(ServerListView *, std::string , std::string ,
                     std::string , std::string , std::string , std::string,
                     QWidget *parent = NULL);
    ServerInfoEntity(ServerListView *, ServerInfoEntity *);
    ~ServerInfoEntity();

    void    init();
    bool    isSelected();
    void    setSelected(bool);
    void    paintEvent(QPaintEvent *);
    void    enterEvent(QEvent *);
    void    leaveEvent(QEvent *);
    std::string getName();
    std::string getMode();
    std::string getMap();
    std::string getPlayerNb();
    std::string getIp();
    std::string getPort();
    std::string getInfoByType(eSortType);


protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent( QMouseEvent * e ) Q_DECL_OVERRIDE;

private:
    std::string _name;
    std::string _mode;
    std::string _map;
    std::string _playerNb;
    std::string _ip;
    std::string _port;

    QLabel      *_nameLbl;
    QLabel      *_modeLbl;
    QLabel      *_mapLbl; 
    QLabel      *_playerNbLbl;

    ServerListView *_sw;

    bool        _selected;
public slots:
    void        __switchSelection();
};

#endif // SERVERINFOENTITY_H
