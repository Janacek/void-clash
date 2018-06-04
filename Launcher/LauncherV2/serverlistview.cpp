#include "serverlistview.h"
#include <iostream>

ServerListView::ServerListView(MainWindow *mw, QWidget *parent)
    : QWidget(parent), _mw(mw), _serverList(NULL), _scrollArea(NULL), _eventLoop(NULL)
{

}

ServerListView::~ServerListView()
{
    delete _serverNameBtn;
    delete _serverModeBtn;
    delete _serverMapBtn;
    delete _serverPlayerNbBtn;
}

void    ServerListView::init()
{
    setVisible(false);
    setFixedSize(640, 600);

    setStyleSheet(QString::fromUtf8("QScrollArea:vertical {"
        "    border: 0px solid #999999;"
        "    background:transparent;"
        "    margin: 0px 0px 0px 0px;"
        "}""QScrollBar:vertical {"
        "    border: 0px solid #999999;"
        "    background: rgb(200, 200, 200);"
        "    width:10px;    "
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop: 0  rgb(80, 80, 80), stop: 0.5 rgb(80, 80, 80),  stop:1 rgb(80, 80, 80));"
        "    min-height: 0px;"
        ""
        "}"
        "QScrollBar::add-line:vertical {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop: 0  rgb(80, 80, 80), stop: 0.5 rgb(80, 80, 80),  stop:1 rgb(80, 80, 80));"
        "    height: 0px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop: 0  rgb(80, 80, 80), stop: 0.5 rgb(80, 80, 80),  stop:1 rgb(80, 80, 80));"
        "    height: 0px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid rgb(160, 196, 35);"
        "    color : rgb(206, 207, 212);"
        "    selection-color: rgb(206, 207, 212);"
        "}"
        "QLineEdit {"
        "    border: 2px solid rgb(160, 196, 35);"
        "    color : rgb(206, 207, 212);"
        "    selection-color: rgb(206, 207, 212);"
        "}"
        ""));
    QFont bold("Gotham Bold");
    bold.setPointSize(12);
    _refreshStatus = new QLabel("Let's make waffles !", this);
    _refreshStatus->setFont(bold);
    _refreshStatus->setStyleSheet("color : rgb(206, 207, 212);"
                                    "background-color:rgb(40, 40, 40, 0);");
    _refreshStatus->move(230, 250);

    _serverNameBtn = new TextButton(QString("nom"), QString("Gotham Light"), 12, this);
    _serverNameBtn->setStrStyle("color : rgb(130, 130, 130);"
                            "background-color:rgb(40, 40, 40, 0);");
    _serverNameBtn->setStrHoverStyle("color : rgb(220, 220, 220);"
                            "background-color:rgb(40, 40, 40, 0);");
    _serverNameBtn->move(60, 120);

    _serverModeBtn = new TextButton(QString("mode"), QString("Gotham Light"), 12, this);
    _serverModeBtn->setStrStyle("color : rgb(130, 130, 130);"
                            "background-color:rgb(40, 40, 40, 0);");
    _serverModeBtn->setStrHoverStyle("color : rgb(220, 220, 220);"
                                     "background-color:rgb(40, 40, 40, 0);");
    _serverModeBtn->move(210, 120);

    _serverPlayerNbBtn = new TextButton(QString("players"), QString("Gotham Light"), 12, this);
    _serverPlayerNbBtn->setStrStyle("color : rgb(130, 130, 130);"
                            "background-color:rgb(40, 40, 40, 0);");
    _serverPlayerNbBtn->setStrHoverStyle("color : rgb(220, 220, 220);"
                                         "background-color:rgb(40, 40, 40, 0);");
    _serverPlayerNbBtn->move(375, 120);

    _serverMapBtn = new TextButton(QString("map"), QString("Gotham Light"), 12, this);
    _serverMapBtn->setStrStyle("color : rgb(130, 130, 130);"
                            "background-color:rgb(40, 40, 40, 0);");
    _serverMapBtn->setStrHoverStyle("color : rgb(220, 220, 220);"
                                    "background-color:rgb(40, 40, 40, 0);");
    _serverMapBtn->move(490, 120);

    _connectBtn = new TextButton(QString("CONNECT"), QString("Gotham Medium"), 10, this);
    _connectBtn->setStyleSheet("color : rgb(206, 206, 214);"
                            "background-color:rgb(40, 40, 40, 0);");
    _connectBtn->move(500, 60);

    _refreshBtn = new ComplexButton(QString("REFRESH"), QString("Gotham Medium"), 10, 0, 10,
                                  QString("../assets/launcher/refresh.png"),
                                  QString("../assets/launcher/refresh_hover.png"),
                                  45, 45, 50, 10, this);
    _refreshBtn->move(45, 45);

    _customIp = new ComplexButton(QString("CUSTOM IP"), QString("Gotham Medium"), 9, 0, 10,
                                  QString("../assets/launcher/lan.png"),
                                  QString("../assets/launcher/lan.png"),
                                  45, 45, 50, 10, this);
    _customIp->move(315, 45);
    _ipInput = new QLineEdit(this);
    _ipInput->move(330, 60);
    _ipInput->setAttribute(Qt::WA_MacShowFocusRect, 0);
    _ipInput->setVisible(false);
    _quitIpInput = new BasicButton(QString("../assets/launcher/quitinput.png"),
                                   QString("../assets/launcher/quitinput_hover.png"),
                                   25, 25, this);
    _quitIpInput->move(440, 57);
    _quitIpInput->setVisible(false);

    QObject::connect(_customIp, SIGNAL(clicked()), this, SLOT(__displayIpInput()));
    QObject::connect(_quitIpInput, SIGNAL(clicked()), this, SLOT(__removeIpInput()));
    QObject::connect(_refreshBtn, SIGNAL(clicked()), this, SLOT(__refreshServerList()));
    QObject::connect(_serverNameBtn, SIGNAL(clicked()), this, SLOT(__sortServersByName()));
    QObject::connect(_serverModeBtn, SIGNAL(clicked()), this, SLOT(__sortServersByMode()));
    QObject::connect(_serverMapBtn, SIGNAL(clicked()), this, SLOT(__sortServersByMap()));
    QObject::connect(_serverPlayerNbBtn, SIGNAL(clicked()), this, SLOT(__sortServersByPlayerNb()));
    QObject::connect(_connectBtn, SIGNAL(clicked()), this, SLOT(__connectToServer()));
}

void    ServerListView::openServerList()
{
    _scrollArea = new QScrollArea(this);
    _serverList = new QWidget(_scrollArea);
    _scrollArea->setFixedSize(600, 400);
    _scrollArea->setWidget(_serverList);
    _scrollArea->move(10, 150);
    setVisible(true);
    _scrollArea->setVisible(true);
    _serverList->setVisible(true);
    __refreshServerList();
}

void    ServerListView::closeServerList()
{
    _clearServerLists();
    if (_serverList) {
        delete _serverList;
        _serverList = NULL;
    }
    if (_scrollArea) {
        delete _scrollArea;
        _scrollArea = NULL;
    }
}

void    ServerListView::unselectedAllServers()
{
    for (unsigned int i = 0; i < _serverListUI.size(); ++i)
    {
        _serverListUI[i]->setSelected(false);
    }
}

bool ServerListView::event(QEvent *event)
{
    if (event->type() == SelectServerEvent::type()) {
        __removeIpInput();
        unselectedAllServers();
    } else if (event->type() == ConnectServerEvent::type())
    {
        __removeIpInput();
        __connectToServer();
        unselectedAllServers();
    }
    return QWidget::event(event);
}

//----------------
// PRIVATE FUNCTIONS
//----------------
void    ServerListView::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

void    ServerListView::_displayServerList()
{
    static int saveSize = 0;
    ServerInfoEntity *tmp;
    for (unsigned int i = saveSize; i < _serverInfos.size(); i += 6)
    {
        tmp = new ServerInfoEntity(this, _serverInfos[i], _serverInfos[i + 1],
                _serverInfos[i + 2], _serverInfos[i + 3], _serverInfos[i + 4], _serverInfos[i + 5],
                _serverList);
        tmp->init();
        tmp->move(0, 10 + 65 * ((i - saveSize) / 6));
        _serverListUI.push_back(tmp);
    }
    _serverList->adjustSize();
    saveSize = _serverInfos.size();
}

void    ServerListView::_getServerList()
{
    _refreshStatus->setText("En recherche de serveurs...");
    _refreshStatus->adjustSize();
    _refreshStatus->setVisible(true);
    //_transformJsonToServerList("[{\"Id\":159,\"Name\":\"dsqd\",\"CurrentMode\":\"qdqd\",\"PlayerNumber\":1,\"MaxPlayer\":1,\"CurrentMap\":\"qdqsdqdqd\",\"IP\":\"qdqsd\",\"DateCreated\":null,\"LastUpdate\":null}, {\"Id\":159,\"Name\":\"dsqd\",\"CurrentMode\":\"qdqd\",\"PlayerNumber\":1,\"MaxPlayer\":1,\"CurrentMap\":\"qdqsdqdqd\",\"IP\":\"qdqsd\",\"DateCreated\":null,\"LastUpdate\":null}]");
    QNetworkAccessManager mgr;
//    if (!_eventLoop)
        _eventLoop = new QEventLoop();
//    else
//        return ;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), _eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("http://voidclashserver.azurewebsites.net/api/server") ) );
    QNetworkReply *reply = mgr.get(req);
    _eventLoop->exec();
    if (reply->error() == QNetworkReply::NoError)
        _transformJsonToServerList(reply->readAll());
    if (reply)
        delete reply;
//    if (_eventLoop->isRunning())
//        _eventLoop->exit();
    //delete _eventLoop;
    //_eventLoop = NULL;
}

void        ServerListView::_transformJsonToServerList(QString jsonStr)
{
    QString buf(jsonStr);
    QString beg("{\"servers\":");
    QString end("}");
    QString finalStr(beg + buf + end);
    QJsonDocument doc = QJsonDocument::fromJson(finalStr.toUtf8());
    QJsonObject obj = doc.object();
    QJsonArray servers = obj["servers"].toArray();
    QJsonObject final;
    for (int i = 0; i < servers.size(); ++i)
    {
        final = servers[i].toObject();
        _serverInfos.push_back(std::string( final["Name"].toString().toStdString() ));
        _serverInfos.push_back(std::string( final["CurrentMode"].toString().toStdString()));
        int playerNumber = final["PlayerNumber"].toInt();
        int maxPlayer = final["MaxPlayer"].toInt();
        std::stringstream pNb;
        std::stringstream mNb;
        pNb << playerNumber;
        mNb << maxPlayer;
        std::string playersInfo = pNb.str() + "/" + mNb.str();
        _serverInfos.push_back(playersInfo);
        _serverInfos.push_back(std::string( final["CurrentMap"].toString().toStdString()));
        _serverInfos.push_back(std::string( final["IP"].toString().toStdString()));
        _serverInfos.push_back(std::string( final["Port"].toString().toStdString()));
    }
}

void        ServerListView::_adaptJsonStr(QString &jsonStr)
{
    std::string final = "{\"servers\":";
    std::string tmp = jsonStr.toStdString();
    final += tmp;
    final += std::string("}");
    jsonStr = QString::fromStdString(final);
}

void        ServerListView::_clearServerLists()
{
    _clearServerListUI();
}

void        ServerListView::_clearServerListUI()
{
        for (unsigned int i = 0; i < _serverListUI.size(); ++i) {
        delete _serverListUI[i];
    }
    _serverListUI.clear();
}

void        ServerListView::_setNewServerList(std::deque<ServerInfoEntity *> newOne)
{
    _scrollArea->setVisible(false);
    _clearServerListUI();
    for (unsigned int i = 0; i < newOne.size(); ++i)
    {
        _serverListUI.push_back(newOne[i]);
        newOne[i]->init();
        newOne[i]->setParent(_serverList);
        newOne[i]->move(0, 10 + 65 * i);
    }
    _scrollArea->adjustSize();
    _scrollArea->setVisible(true);
}

void        ServerListView::_sortServersByInfo(eSortType type)
{
    if (!_serverListUI.size())
        return;
    std::vector <std::string> toSort;
    for (unsigned int i = 0; i < _serverListUI.size(); ++i)
    {
        std::string tmp = _serverListUI[i]->getInfoByType(type);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
        toSort.push_back(tmp);
    }
    std::sort(toSort.begin(), toSort.end());
    if (_lastSort == type) {
        std::reverse(toSort.begin(), toSort.end());
        _lastSort = NONESORT;
    } else
        _lastSort = type;
    std::deque<ServerInfoEntity *> newOne;
    unsigned int pos = 0;
    int count = 0;
    while (_serverListUI.size())
    {
        if (pos == _serverListUI.size())
            pos = 0;
        std::string tmp = _serverListUI[pos]->getInfoByType(type);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
        if (toSort[count] == tmp) {
            newOne.push_back(new ServerInfoEntity(this, _serverListUI[pos]));
            delete _serverListUI[pos];
            _serverListUI.erase(_serverListUI.begin() + pos);
            ++count;
            pos = -1;
        }
        ++pos;
    }
    _setNewServerList(newOne);
}

//----------------
// PUBLIC SLOTS
//----------------
void        ServerListView::__refreshServerList()
{
    _clearServerLists();
    _scrollArea->setVisible(false);
    _getServerList();
    if (_serverInfos.size()) {
        _displayServerList();
        _refreshStatus->setVisible(false);
    }
    else
        _refreshStatus->setText("Pas de serveurs disponibles...");
    _refreshStatus->adjustSize();
    if (_scrollArea)
        _scrollArea->setVisible(true);
}

void        ServerListView::__sortServersByName()
{
    _sortServersByInfo(NAMESORT);
}

void        ServerListView::__sortServersByMode()
{
    _sortServersByInfo(MODESORT);
}

void        ServerListView::__sortServersByMap()
{
    _sortServersByInfo(MAPSORT);
}

void        ServerListView::__sortServersByPlayerNb()
{
    _sortServersByInfo(PLAYERNBSORT);
}

std::string ServerListView::_getServerIP()
{
    for (unsigned int i = 0; i < _serverListUI.size(); ++i)
    {
        if (_serverListUI[i]->isSelected())
            return std::string(_serverListUI[i]->getIp() + " " + _serverListUI[i]->getPort());
    }
    return std::string("");
}

void        ServerListView::__connectToServer()
{
    std::string ipStr;
    if (!_ipInput->text().size())
    {
        ipStr = _getServerIP();
        if (!ipStr.size() || ipStr == "null")
            return ;
    } else {
        ipStr = _ipInput->text().toStdString();
    }
#ifdef __unix__
    std::string tmp = "./client " + ipStr;
#elif defined(_WIN32) || defined(WIN32)
    std::string tmp = "./client.exe " + ipStr;
#endif
    QProcess *process = new QProcess(this);
    QString file(tmp.c_str());
    process->start(file);
    process->waitForStarted();
    process->waitForFinished();
}

void        ServerListView::__displayIpInput()
{
    unselectedAllServers();
    _ipInput->setVisible(true);
    _quitIpInput->setVisible(true);
    _customIp->setVisible(false);
}

void        ServerListView::__removeIpInput()
{
    _ipInput->setVisible(false);
    _ipInput->clear();
    _quitIpInput->setVisible(false);
    _customIp->setVisible(true);
}
