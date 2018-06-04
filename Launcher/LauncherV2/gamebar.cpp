#include "gamebar.h"

GameBar::GameBar(QWidget *parent): Field(parent)
{

}

GameBar::~GameBar()
{

}

void GameBar::init()
{
    setStyleSheet("background-color:rgb(10, 10, 10, 200)");
    _initLayouts();
}

void GameBar::unselectedButtons()
{
    _gameConfigBtn->setSelected(false);
    _optionBtn->setSelected(false);
}

void GameBar::selectGameConfig()
{
    _gameConfigBtn->setSelected(true);
}

void GameBar::updateInfos(std::map<eInfoType, eSelectionType> &info)
{
    _updateMatchMode(info[MMMODE]);
//    _updateWeapons(info[WEAPON1], info[WEAPON2]);
    _updateGameMode(info[GAMEMODE]);
}

//-----------------
//Private Functions
//-----------------

void GameBar::_initTmpLayout()
{
    _tmpLayout = new TextButton(QString("MATCHMAKING - BIENTOT DISPONIBLE"), QString("Gotham Bold"), 15, this);
    _tmpLayout->adjustSize();
    _tmpLayout->setStyleSheet("background-color:rgb(35, 35, 35, 200);");
    _tmpLayout->setStrStyle("color : rgb(206, 206, 214);background-color:rgb(35, 35, 35, 200);");
    _tmpLayout->setStrHoverStyle("color : rgb(206, 206, 214);background-color:rgb(35, 35, 35, 200);");
    _tmpLayout->move(0, 0);
    _tmpLayout->resize(560, 140);
}

void GameBar::_initQuickGameButton()
{
    _quickMatch = new BasicButton(QString("../assets/launcher/fight.png"),
                                  QString("../assets/launcher/fight.png"),
                                  20, 130);
    _quickMatch->setStyleSheet("background-color:rgb(10, 10, 10, 200);"
                               "border-style: outset;"
                               "border-width: 4px;"
                              " border-color: rgb(206, 239, 12);");
    _gridLayout->addWidget(_quickMatch, 0, 0, 2, 1);

    //QObject::connect(_quickMatch, SIGNAL(clicked()),this, SLOT(__launchGame()));
}

void GameBar::_initGameSettings()
{
    //Labels Area
    _labelRankedTypeGrp = new QWidget();
    _labelRankedTypeGrp->setStyleSheet("background-color:rgb(30, 30, 30, 0);");
    _starImage = new BasicButton(QString("../assets/launcher/star.png"),
                                 QString("../assets/launcher/star.png"),
                                 15, 15);
    _starImage->setParent(_labelRankedTypeGrp);
    _starImage->move(10, 20);

    QFont black("Gotham Black");
    _labelRankedType1 = new QLabel("INCREDIBLE MATCH", _labelRankedTypeGrp);
    _labelRankedType1->setFont(black);
    _labelRankedType1->setStyleSheet("color : rgb(135, 135, 135);");
    _labelRankedType1->move(35, 25);

    QFont light("Gotham Light");
    _labelRankedType2 = new QLabel("game type", _labelRankedTypeGrp);
    _labelRankedType2->setFont(light);
    _labelRankedType2->setStyleSheet("color : rgb(89, 89, 89);");
    _labelRankedType2->move(35, 40);

    _gridLayout->addWidget(_labelRankedTypeGrp, 0, 1, 2, 2);

    //Buttons Area
    _gameTypeChoiceGrp = new QWidget();
    _gameTypeChoiceGrp->setStyleSheet("background-color:rgb(30, 30, 30, 0);");
    _DMBtn = new BasicButton(QString("../assets/launcher/dm.png"),
                             QString("../assets/launcher/dm.png"),
                             35, 35);
    _DMBtn->setParent(_gameTypeChoiceGrp);
    _DMBtn->move(25, 0);
    _FFABtn = new BasicButton(QString("../assets/launcher/ffa.png"),
                             QString("../assets/launcher/ffa.png"),
                             35, 35);
    _FFABtn->setParent(_gameTypeChoiceGrp);
    _FFABtn->move(60, 0);
    _CTFBtn = new BasicButton(QString("../assets/launcher/ctf.png"),
                             QString("../assets/launcher/ctf.png"),
                             35, 35);
    _CTFBtn->move(95, 0);
    _CTFBtn->setParent(_gameTypeChoiceGrp);
    _TSVRBtn = new BasicButton(QString("../assets/launcher/tsvr.png"),
                             QString("../assets/launcher/tsvr.png"),
                             35, 35);
    _TSVRBtn->move(130, 0);
    _TSVRBtn->setParent(_gameTypeChoiceGrp);
    _HORBtn = new BasicButton(QString("../assets/launcher/hor.png"),
                             QString("../assets/launcher/hor.png"),
                             35, 35);
    _HORBtn->move(165, 0);
    _HORBtn->setParent(_gameTypeChoiceGrp);
    _KOTHBtn = new BasicButton(QString("../assets/launcher/koth.png"),
                             QString("../assets/launcher/koth.png"),
                             35, 35);
    _KOTHBtn->move(200, 0);
    _KOTHBtn->setParent(_gameTypeChoiceGrp);

    _labelRankedMode = new QLabel("game mode", _gameTypeChoiceGrp);
    _labelRankedMode->setFont(light);
    _labelRankedMode->setStyleSheet("color : rgb(89, 89, 89);");
    _labelRankedMode->move(35, 35);

    _gridLayout->addWidget(_gameTypeChoiceGrp, 1, 1, 2, 2);

    //Weapons Area
//    _weaponsGrp = new QWidget();
//    _weaponsGrp->setStyleSheet("background-color:rgb(30, 30, 30, 0);");
//    _chosenWeapons = new QLabel("GRAINES DE TOURNESOL", _weaponsGrp);
//    _chosenWeapons->setFont(black);
//    _chosenWeapons->setStyleSheet("color : rgb(135, 135, 135);");
//    _chosenWeapons->move(35, 25);
//    _labelWeapon = new QLabel("weapons", _weaponsGrp);
//    _labelWeapon->setFont(light);
//    _labelWeapon->setStyleSheet("color : rgb(89, 89, 89);");
//    _labelWeapon->move(35, 40);

//    _gridLayout->addWidget(_weaponsGrp, 0, 2, 2, 2);
}

void GameBar::_initGameMenu()
{
    _gameMenu = new Field();
    _gameMenu->setStyleSheet("background-color:rgb(30, 30, 30, 0);");
    _gameMenu->init();

    _gameConfigBtn = new BasicButton(QString("../assets/launcher/flash.png"),
                                 QString("../assets/launcher/flash_hover.png"),
                                 30, 30);
    _optionBtn = new BasicButton(QString("../assets/launcher/settings.png"),
                                 QString("../assets/launcher/settings_hover.png"),
                                 35, 35);
    _quitBtn = new BasicButton(QString("../assets/launcher/on.png"),
                                 QString("../assets/launcher/on_hover.png"),
                                 35, 35);
    _gameMenu->addToLayout(_gameConfigBtn, 0, 0, 1, 1);
    _gameMenu->addToLayout(_optionBtn, 0, 2, 1, 1);
    _gameMenu->addToLayout(_quitBtn, 0, 3, 1, 1);
    _gridLayout->addWidget(_gameMenu, 0, 3, 3, 2);

    //QObject::connect(_gameConfigBtn, SIGNAL(clicked()), this->parent(), SLOT(__activateWeapons()));
    QObject::connect(_quitBtn, SIGNAL(clicked()),this, SLOT(__quitGame()));
}

void GameBar::_initLayouts()
{
    _initQuickGameButton();
    _initGameSettings();
    _initGameMenu();
    _initTmpLayout();
}

void GameBar::_updateMatchMode(eSelectionType newMode)
{
    if (newMode == NMATCHBTN)
        _labelRankedType1->setText(QString("NORMAL MATCH"));
    else
        _labelRankedType1->setText(QString("RANKED MATCH"));
    _labelRankedType1->adjustSize();
}

//void GameBar::_updateWeapons(eSelectionType weapon1, eSelectionType weapon2)
//{
//    std::map<eSelectionType, std::string> dico;

//    dico[BOMBBTN] = std::string("BOMB");
//    dico[TURRETBTN] = std::string("TURRET");
//    dico[BULBTN] = std::string("BULLET");
//    dico[GRAVITYBTN] = std::string("GRAVITY");
//    dico[ROCKETBTN] = std::string("ROCKET");
//    std::string tmp = dico[weapon1] + ", " + dico[weapon2];
//    _chosenWeapons->setText(QString::fromStdString(tmp));
//    _chosenWeapons->adjustSize();

//}

void GameBar::_updateGameMode(eSelectionType newMode)
{
    _DMBtn->setSelected(false);
    _FFABtn->setSelected(false);
    _CTFBtn->setSelected(false);
    _TSVRBtn->setSelected(false);
    _HORBtn->setSelected(false);
    _KOTHBtn->setSelected(false);

    if (newMode == DMBTN)
        _DMBtn->setSelected(true);
    if (newMode == FFABTN)
        _FFABtn->setSelected(true);
    if (newMode == CTFBTN)
        _CTFBtn->setSelected(true);
    if (newMode == TSVRBTN)
        _TSVRBtn->setSelected(true);
    if (newMode == HORBTN)
        _HORBtn->setSelected(true);
    if (newMode == KOTHBTN)
        _KOTHBtn->setSelected(true);
}

#include <iostream>

//PUBLIC SLOTS (Signal buttons)
void GameBar::__launchGame()
{
    QProcess *process = new QProcess(this);
    QString save = QDir::currentPath();
    QDir::setCurrent(QString("../../"));
    QString file = "./client";
    process->start(file);
    process->waitForStarted();
    process->waitForFinished();
    QDir::setCurrent(save);
}

void GameBar::__quitGame()
{
    QApplication::quit();
}

