#ifndef CHOICEVIEW_H
#define CHOICEVIEW_H

#include <QCoreApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>

#include "basicbutton.h"
#include "textbutton.h"
#include "fixedimage.h"
#include "common.h"
#include "refreshinfosevent.h"
#include "gameconfigview.h"
#include "serverlistview.h"

class MainWindow;

class ChoiceView: public QWidget
{
    Q_OBJECT
public:
    ChoiceView(MainWindow *parent = NULL);
    ~ChoiceView();

    void init();
    void activateHome();
    void activateGameConfig(std::map<eInfoType, eSelectionType> &);
    void activateServerList();
    void desactivateAllViews();

    void refreshInfos(RefreshInfosEvent *);

    void paintEvent(QPaintEvent *);

private:
    void    _initLayouts();

    //Home
    void    _initHome();

    MainWindow *_mw;

    QWidget *_homeView;
    QWidget *_carousel;
    FixedImage *_voidClashBg;

    GameConfigView *_gameConfig;
    ServerListView *_serverList;
};

#endif // CHOICEVIEW_H
