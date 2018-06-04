#include "mainwindow.h"
#include <QLabel>
#include <QApplication>
#include <QDir>
#include <iostream>
#include <fstream>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setWindowFlags( Qt::CustomizeWindowHint );
    setWindowTitle("Void Clash Launcher");
    setFixedSize(1280, 720);
}

MainWindow::~MainWindow()
{
    delete _socialArea;
    delete _choiceView;
    delete _gameBar;
    delete _toolsBar;
    delete _mainField;
}

void MainWindow::_initLayouts()
{
   //Init Main Layout (GRID)
   _mainField = new Field(this);
   _mainField->init();
   setCentralWidget(static_cast<QWidget *>(_mainField));

   //Init Tools Bar
   _toolsBar = new ToolsBar(this);
   _toolsBar->init();
   _mainField->addToLayout(_toolsBar, 0, 0, 1, 1);

   //Init Game Bar
   _gameBar = new GameBar(this);
   _gameBar->init();
   _mainField->addToLayout(_gameBar, 0, 1, 1, 1);

   //Init Choice View
   _choiceView = new ChoiceView(this);
   _choiceView->init();
   _mainField->addToLayout(_choiceView, 1, 0, 4, 1);
   installEventFilter(_choiceView);

   //Init Social Area
   _socialArea = new SocialArea(this);
   _socialArea->init();
   _mainField->addToLayout(_socialArea, 1, 1, 4, 1);
}

void MainWindow::start()
{
    //Add fonts
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Black.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-BlackItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Bold.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-BoldItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Book.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-BookItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Light.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-LightItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Medium.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-MediumItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Thin.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-ThinItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Ultra.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-UltraItalic.ttf");
    QFontDatabase::addApplicationFont("../assets/launcher/fonts/gotham/Gotham-Black.ttf");

    //Set Background
    QPixmap bkgnd("../assets/launcher/background.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    _initLayouts();

    _initGeneralInfos();
}

void MainWindow::_initGeneralInfos()
{
    _generalInfos[MMMODE] = RMATCHBTN;
    _generalInfos[GAMEMODE] = DMBTN;
//    _generalInfos[WEAPON1] = BOMBBTN;
//    _generalInfos[WEAPON2]= TURRETBTN;
    _gameBar->updateInfos(_generalInfos);
}

void MainWindow::setGeneralInfo(eInfoType infoType, eSelectionType info)
{
    _generalInfos[infoType] = info;
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == RefreshInfosEvent::type()) {
        RefreshInfosEvent *info = static_cast<RefreshInfosEvent *>(event);
        setGeneralInfo(info->getType(), info->getInfo());
        _gameBar->updateInfos(_generalInfos);
        return true;
    }
    return QWidget::event(event);
}

//PUBLIC SLOTS (Signal Buttons)
void MainWindow::__activateWeapons()
{
    _toolsBar->unselectedButtons();
    _gameBar->unselectedButtons();
    _gameBar->selectGameConfig();
    _choiceView->activateGameConfig(_generalInfos);
}

void MainWindow::__activateServerList()
{
    _toolsBar->unselectedButtons();
    _gameBar->unselectedButtons();
    _toolsBar->selectServerList();
    _choiceView->activateServerList();
}

void MainWindow::__activateNews()
{
    _toolsBar->unselectedButtons();
    _gameBar->unselectedButtons();
    _toolsBar->selectNews();
    _choiceView->activateHome();
}

