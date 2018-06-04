#include "gameconfigview.h"

GameConfigView::GameConfigView(MainWindow *mw, QWidget *parent)
    : QWidget(parent), _mw(mw)
{
}

GameConfigView::~GameConfigView()
{
    delete _gameSettingsLbl;
    delete _gameTypeLbl;
    delete _gameModeLbl;
//    delete _weaponsLbl;

    delete _textButtons[NMATCHBTN];
    delete _textButtons[RMATCHBTN];
    delete _textButtons[DMBTN];
    delete _textButtons[CTFBTN];
    delete _textButtons[FFABTN];
    delete _textButtons[HORBTN];
    delete _textButtons[TSVRBTN];
    delete _textButtons[KOTHBTN];
//    delete _textButtons[BOMBBTN];
//    delete _textButtons[BULBTN];
//    delete _textButtons[GRAVITYBTN];
//    delete _textButtons[TURRETBTN];
//    delete _textButtons[ROCKETBTN];
}

void    GameConfigView::init()
{
    setVisible(false);
    setFixedSize(640, 600);

    QFont medium("Gotham Medium");
    QFont light("Gotham Light");

    medium.setPointSize(15);
    _gameSettingsLbl = new QLabel("Game Settings", this);
    _gameSettingsLbl->setFont(medium);
    _gameSettingsLbl->setStyleSheet("color : rgb(206, 207, 212);"
                                    "background-color:rgb(40, 40, 40, 0);");
    _gameSettingsLbl->move(250, 50);

    light.setPointSize(10);
    _gameTypeLbl = new QLabel("GAME TYPE", this);
    _gameTypeLbl->setFont(light);
    _gameTypeLbl->setStyleSheet("color : rgb(130, 130, 130);"
                                    "background-color:rgb(40, 40, 40, 0);");
    _gameTypeLbl->move(70, 125);
    _textButtons[NMATCHBTN] = new TextButton(QString("NORMAL MATCH"), QString("Gotham Bold"), 10, this);
    _textButtons[NMATCHBTN]->move(65, 160);
    _textButtons[RMATCHBTN] = new TextButton(QString("RANKED MATCH"), QString("Gotham Bold"), 10, this);
    _textButtons[RMATCHBTN]->move(65, 195);

    _gameModeLbl = new QLabel("GAME MODE", this);
    _gameModeLbl->setFont(light);
    _gameModeLbl->setStyleSheet("color : rgb(130, 130, 130);"
                                    "background-color:rgb(40, 40, 40, 0);");
    _gameModeLbl->move(270, 125);
    _textButtons[DMBTN] = new TextButton(QString("Deathmatch"), QString("Gotham Bold"), 10, this);
    _textButtons[DMBTN]->move(265, 160);
    _textButtons[FFABTN] = new TextButton(QString("Free For All"), QString("Gotham Bold"), 10, this);
    _textButtons[FFABTN]->move(265, 195);
    _textButtons[CTFBTN] = new TextButton(QString("Capture The Flag"), QString("Gotham Bold"), 10, this);
    _textButtons[CTFBTN]->move(265, 230);
    _textButtons[TSVRBTN] = new TextButton(QString("Team Survivor"), QString("Gotham Bold"), 10, this);
    _textButtons[TSVRBTN]->move(265, 265);
    _textButtons[HORBTN] = new TextButton(QString("Horde"), QString("Gotham Bold"), 10, this);
    _textButtons[HORBTN]->move(250, 300);
    _textButtons[KOTHBTN] = new TextButton(QString("King Of The Hill"), QString("Gotham Bold"), 10, this);
    _textButtons[KOTHBTN]->move(265, 335);

//    _weaponsLbl = new QLabel("WEAPONS", this);
//    _weaponsLbl->setFont(light);
//    _weaponsLbl->setStyleSheet("color : rgb(130, 130, 130);"
//                                    "background-color:rgb(40, 40, 40, 0);");
//    _weaponsLbl->move(470, 125);
//    _textButtons[BOMBBTN] = new TextButton(QString("Bomb"), QString("Gotham Bold"), 10, this);
//    _textButtons[BOMBBTN]->move(450, 160);
//    _textButtons[BULBTN] = new TextButton(QString("Bullet"), QString("Gotham Bold"), 10, this);
//    _textButtons[BULBTN]->move(450, 195);
//    _textButtons[GRAVITYBTN] = new TextButton(QString("Gravity Gun"), QString("Gotham Bold"), 10, this);
//    _textButtons[GRAVITYBTN]->move(460, 230);
//    _textButtons[ROCKETBTN] = new TextButton(QString("Rocket"), QString("Gotham Bold"), 10, this);
//    _textButtons[ROCKETBTN]->move(450, 265);
//    _textButtons[TURRETBTN] = new TextButton(QString("Turret"), QString("Gotham Bold"), 10, this);
//    _textButtons[TURRETBTN]->move(450, 300);

    QObject::connect(_textButtons[NMATCHBTN], SIGNAL(clicked()), this, SLOT(__normalMatchCB()));
    QObject::connect(_textButtons[RMATCHBTN], SIGNAL(clicked()), this, SLOT(__rankedMatchCB()));
    QObject::connect(_textButtons[DMBTN], SIGNAL(clicked()), this, SLOT(__dmCB()));
    QObject::connect(_textButtons[FFABTN], SIGNAL(clicked()), this, SLOT(__ffaCB()));
    QObject::connect(_textButtons[CTFBTN], SIGNAL(clicked()), this, SLOT(__ctfCB()));
    QObject::connect(_textButtons[TSVRBTN], SIGNAL(clicked()), this, SLOT(__tsvrCB()));
    QObject::connect(_textButtons[HORBTN], SIGNAL(clicked()), this, SLOT(__horCB()));
    QObject::connect(_textButtons[KOTHBTN], SIGNAL(clicked()), this, SLOT(__kothCB()));
//    QObject::connect(_textButtons[BOMBBTN], SIGNAL(clicked()), this, SLOT(__bombCB()));
//    QObject::connect(_textButtons[BULBTN], SIGNAL(clicked()), this, SLOT(__bulletCB()));
//    QObject::connect(_textButtons[GRAVITYBTN], SIGNAL(clicked()), this, SLOT(__gravityCB()));
//    QObject::connect(_textButtons[ROCKETBTN], SIGNAL(clicked()), this, SLOT(__rocketCB()));
//    QObject::connect(_textButtons[TURRETBTN], SIGNAL(clicked()), this, SLOT(__turretCB()));

}

void    GameConfigView::initConfigSelection(std::map<eInfoType, eSelectionType> &infos)
{
    _textButtons[infos[MMMODE]]->setSelected(true);
    _textButtons[infos[GAMEMODE]]->setSelected(true);
//    _textButtons[infos[WEAPON1]]->setSelected(true);
//    _textButtons[infos[WEAPON2]]->setSelected(true);
}

//----------------
// PRIVATE FUNCTIONS
//----------------
void GameConfigView::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

//----------------
//PUBLIC SLOTS (Signal Buttons)
//----------------

void GameConfigView::__normalMatchCB()
{
    _textButtons[NMATCHBTN]->switchSelection();
    _changeMatch(_textButtons[NMATCHBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(MMMODE, NMATCHBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__rankedMatchCB()
{
    _textButtons[RMATCHBTN]->switchSelection();
    _changeMatch(_textButtons[RMATCHBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(MMMODE, RMATCHBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__dmCB()
{
    _textButtons[DMBTN]->switchSelection();
    _changeMode(_textButtons[DMBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(GAMEMODE, DMBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__ffaCB()
{
    _textButtons[FFABTN]->switchSelection();
    _changeMode(_textButtons[FFABTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(GAMEMODE, FFABTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__ctfCB()
{
    _textButtons[CTFBTN]->switchSelection();
    _changeMode(_textButtons[CTFBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(GAMEMODE, CTFBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__tsvrCB()
{
    _textButtons[TSVRBTN]->switchSelection();
    _changeMode(_textButtons[TSVRBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(GAMEMODE, TSVRBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__horCB()
{
    _textButtons[HORBTN]->switchSelection();
    _changeMode(_textButtons[HORBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(GAMEMODE, HORBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

void GameConfigView::__kothCB()
{
    _textButtons[KOTHBTN]->switchSelection();
    _changeMode(_textButtons[KOTHBTN]);
    RefreshInfosEvent * event = new RefreshInfosEvent(GAMEMODE, KOTHBTN);
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

//void GameConfigView::__bombCB()
//{
//    _textButtons[BOMBBTN]->switchSelection();
//    _changeWeapon(_textButtons[BOMBBTN]);
//    RefreshInfosEvent * event = new RefreshInfosEvent(WEAPON1, BOMBBTN);
//    QCoreApplication::sendEvent((QObject*)_mw, event);
//}

//void GameConfigView::__bulletCB()
//{
//    _textButtons[BULBTN]->switchSelection();
//    _changeWeapon(_textButtons[BULBTN]);
//    RefreshInfosEvent * event = new RefreshInfosEvent(WEAPON1, BULBTN);
//    QCoreApplication::sendEvent((QObject*)_mw, event);
//}

//void GameConfigView::__gravityCB()
//{
//    _textButtons[GRAVITYBTN]->switchSelection();
//    _changeWeapon(_textButtons[GRAVITYBTN]);
//    RefreshInfosEvent * event = new RefreshInfosEvent(WEAPON1, GRAVITYBTN);
//    QCoreApplication::sendEvent((QObject*)_mw, event);
//}

//void GameConfigView::__rocketCB()
//{
//    _textButtons[ROCKETBTN]->switchSelection();
//    _changeWeapon(_textButtons[ROCKETBTN]);
//    RefreshInfosEvent * event = new RefreshInfosEvent(WEAPON1, ROCKETBTN);
//    QCoreApplication::sendEvent((QObject*)_mw, event);
//}

//void GameConfigView::__turretCB()
//{
//    _textButtons[TURRETBTN]->switchSelection();
//    _changeWeapon(_textButtons[TURRETBTN]);
//    RefreshInfosEvent * event = new RefreshInfosEvent(WEAPON1, TURRETBTN);
//    QCoreApplication::sendEvent((QObject*)_mw, event);
//}

void GameConfigView::_changeMatch(TextButton *newChoice)
{
    if (_textButtons[NMATCHBTN] != newChoice)
        _textButtons[NMATCHBTN]->setSelected(false);
    if (_textButtons[RMATCHBTN] != newChoice)
        _textButtons[RMATCHBTN]->setSelected(false);
}

void GameConfigView::_changeMode(TextButton *newChoice)
{
    if (_textButtons[DMBTN] != newChoice)
        _textButtons[DMBTN]->setSelected(false);
    if (_textButtons[FFABTN] != newChoice)
        _textButtons[FFABTN]->setSelected(false);
    if (_textButtons[CTFBTN] != newChoice)
        _textButtons[CTFBTN]->setSelected(false);
    if (_textButtons[TSVRBTN] != newChoice)
        _textButtons[TSVRBTN]->setSelected(false);
    if (_textButtons[HORBTN] != newChoice)
        _textButtons[HORBTN]->setSelected(false);
    if (_textButtons[KOTHBTN] != newChoice)
        _textButtons[KOTHBTN]->setSelected(false);

}

//void GameConfigView::_changeWeapon(TextButton *newChoice)
//{
//    if (_textButtons[BOMBBTN] != newChoice)
//        _textButtons[BOMBBTN]->setSelected(false);
//    if (_textButtons[BULBTN] != newChoice)
//        _textButtons[BULBTN]->setSelected(false);
//    if (_textButtons[GRAVITYBTN] != newChoice)
//        _textButtons[GRAVITYBTN]->setSelected(false);
//    if (_textButtons[ROCKETBTN] != newChoice)
//        _textButtons[ROCKETBTN]->setSelected(false);
//    if (_textButtons[TURRETBTN] != newChoice)
//        _textButtons[TURRETBTN]->setSelected(false);
//}
