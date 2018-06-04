#include "choiceview.h"

ChoiceView::ChoiceView(MainWindow *parent)
    : QWidget()
{
    _mw = parent;
}

ChoiceView::~ChoiceView()
{

}

void    ChoiceView::init()
{
    setFixedSize(640, 600);
    setStyleSheet("background-color:rgb(0, 0, 0, 140)");
    _initLayouts();
}

void    ChoiceView::activateHome()
{
    desactivateAllViews();
    _homeView->setVisible(true);
}

void    ChoiceView::activateServerList()
{
    desactivateAllViews();
    _serverList->openServerList();
    _serverList->setVisible(true);
}

void    ChoiceView::activateGameConfig(std::map<eInfoType, eSelectionType> &infos)
{
    desactivateAllViews();
    _gameConfig->setVisible(true);
    _gameConfig->initConfigSelection(infos);
}

void    ChoiceView::desactivateAllViews()
{
    _gameConfig->setVisible(false);
    _homeView->setVisible(false);
    _serverList->setVisible(false);
    _serverList->closeServerList();
}

void    ChoiceView::refreshInfos(RefreshInfosEvent *event)
{
    QCoreApplication::sendEvent((QObject*)_mw, event);
}

//----------------
// PRIVATE FUNCTIONS
//----------------
void ChoiceView::_initHome()
{
    _homeView = new QWidget(this);
    _homeView->setStyleSheet("background-color:rgb(40, 40, 40, 0);");
    _homeView->setFixedSize(640, 600);
//    _carousel = new QWidget(_homeView);
//    _carousel->setStyleSheet("background-color:rgb(40, 40, 40, 0);");
//    _carousel->resize(640, 600);
//    _carousel->move(0, 0);
//    QPixmap pic("../assets/launcher/logo-voidclash.jpg");
//    QPixmap scaled=pic.scaled (360, 360, Qt::KeepAspectRatio, Qt::FastTransformation );

//    QLabel *label = new QLabel(_carousel);
//    label->setPixmap(scaled);
//    label->move(140, 0);

}

void    ChoiceView::_initLayouts()
{
    _initHome();

    //INIT GAME CONFIG VIEW
    _gameConfig = new GameConfigView(_mw, this);
    _gameConfig->init();

    //INIT SERVER LIST VIEW
    _serverList = new ServerListView(_mw, this);
    _serverList->init();

    //Home is the default view
    //activateHome();
    activateServerList();
}

void ChoiceView::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }
