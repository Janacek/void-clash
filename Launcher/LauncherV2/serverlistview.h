#ifndef SERVERLISTVIEW_H
#define SERVERLISTVIEW_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QUrlQuery>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QScrollArea>
#include <deque>
#include <QLabel>
#include <QProcess>
#include <QDir>
#include <QLineEdit>
#include "textbutton.h"
#include "serverinfoentity.h"
#include "complexbutton.h"
#include "basicbutton.h"
#include <sstream>
#include "selectserverevent.h"

class MainWindow;

class ServerListView: public QWidget
{
    Q_OBJECT
public:
    ServerListView(MainWindow *mw, QWidget *parent = NULL);
    ~ServerListView();

    void    init();
    void    openServerList();
    void    closeServerList();
    void    unselectedAllServers();

    void    paintEvent(QPaintEvent *);
    bool    event(QEvent *);
private:
    void    _adaptJsonStr(QString &);
    void    _transformJsonToServerList(QString);
    void    _getServerList();
    void    _displayServerList();
    void    _clearServerLists();
    void    _clearServerListUI();
    void    _setNewServerList(std::deque<ServerInfoEntity *>);
    void    _sortServersByInfo(eSortType);
    std::string    _getServerIP();

    MainWindow *_mw;

    QScrollArea *_scrollArea;
    QWidget *_serverList;

    std::deque<std::string> _serverInfos;
    std::deque<ServerInfoEntity *> _serverListUI;

    TextButton  *_serverNameBtn;
    TextButton  *_serverModeBtn;
    TextButton  *_serverPlayerNbBtn;
    TextButton  *_serverMapBtn;

    QLabel      *_refreshStatus;

    TextButton  *_connectBtn;
    ComplexButton *_refreshBtn;
    ComplexButton *_customIp;
    QLineEdit     *_ipInput;
    BasicButton   *_quitIpInput;

    eSortType   _lastSort;

    QEventLoop *_eventLoop;
public slots:
    void __refreshServerList();
    void __displayIpInput();
    void __removeIpInput();
    void __sortServersByName();
    void __sortServersByMode();
    void __sortServersByMap();
    void __sortServersByPlayerNb();
    void __connectToServer();
};

#endif // SERVERLISTVIEW_H
