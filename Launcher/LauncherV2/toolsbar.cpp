#include "toolsbar.h"

ToolsBar::ToolsBar(QWidget *parent): Field(parent)
{

}

ToolsBar::~ToolsBar()
{
    delete _leftButtons;
    delete _rightButtons;
    delete _serverListButton;
}

void ToolsBar::init()
{
    setStyleSheet("background-color:rgba(10, 10, 10, 200)");
    _initLayouts();
}

void ToolsBar::unselectedButtons()
{
    _newsBtn->setSelected(false);
    _serverListButton->setSelected(false);
}

void ToolsBar::selectNews()
{
    _newsBtn->setSelected(true);
}

void ToolsBar::selectServerList()
{
    _serverListButton->setSelected(true);
}

//-------------------
// PRIVATE FUNCTIONS
//-------------------

void ToolsBar::_initLeftButtons()
{
    _leftButtons = new QWidget(this);
    _leftButtons->setFixedSize(440, 140);

    _newsBtn = new BasicButton(QString("../assets/launcher/minilogo.png"),
                               QString("../assets/launcher/minilogo.png"),
                               120, 120, _leftButtons);
    _newsBtn->move(45, 10);

    _gridLayout->addWidget(_leftButtons, 0, 0, 1, 2);

    //QObject::connect(_newsBtn, SIGNAL(clicked()), this->parent(), SLOT(__activateNews()));
}

void ToolsBar::_initRightButtons()
{
    _rightButtons = new Field();
    _rightButtons->init();

    _serverListButton = new BasicButton(QString("../assets/launcher/server.png"),
                                        QString("../assets/launcher/server_hover.png"),
                                        30, 30);
    _mapEditorButton = new BasicButton(QString("../assets/launcher/map.png"),
                                        QString("../assets/launcher/map_hover.png"),
                                        30, 30);
    _rightButtons->addToLayout(_serverListButton, 0, 2, 1, 1);
    _rightButtons->addToLayout(_mapEditorButton, 0, 1, 1, 1);
    _gridLayout->addWidget(_rightButtons, 0, 2, 1, 1);

    QObject::connect(_mapEditorButton, SIGNAL(clicked()),this, SLOT(__launchMapEditor()));
    QObject::connect(_serverListButton, SIGNAL(clicked()),this->parent(), SLOT(__activateServerList()));
}

void ToolsBar::_initLayouts()
{
    _initLeftButtons();
    _initRightButtons();
}

#include <iostream>

//PUBLIC SLOTS (Signal button)
void ToolsBar::__launchMapEditor()
{
    QProcess *process = new QProcess(this);
//    QString save = QDir::currentPath();
//    std::cout << save.toStdString() << std::endl;
    //QDir::setCurrent(QString("../../"));
    QString file = "./NewMapEditor.exe";
    process->start(file);
//    process->waitForStarted();
//    process->waitForFinished();
    //QDir::setCurrent(save);
}
