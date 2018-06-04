#ifndef GAMECONFIGVIEW_H
#define GAMECONFIGVIEW_H

#include <QLabel>
#include <QCoreApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include "common.h"
#include "textbutton.h"
#include "refreshinfosevent.h"

class MainWindow;

class GameConfigView : public QWidget
{
    Q_OBJECT
public:
    GameConfigView(MainWindow *mw, QWidget *parent = NULL);
    ~GameConfigView();

    void    init();
    void    initConfigSelection(std::map<eInfoType, eSelectionType> &);

    void    paintEvent(QPaintEvent *);
private:
    void    _changeMatch(TextButton *);
    void    _changeMode(TextButton *);
//    void    _changeWeapon(TextButton *);

    QLabel  *_gameSettingsLbl;
    QLabel  *_gameTypeLbl;
    QLabel  *_gameModeLbl;
//    QLabel  *_weaponsLbl;

    std::map<eSelectionType, TextButton*> _textButtons;

    MainWindow *_mw;

public slots:
    void __normalMatchCB();
    void __rankedMatchCB();
    void __dmCB();
    void __ffaCB();
    void __ctfCB();
    void __tsvrCB();
    void __horCB();
    void __kothCB();
//    void __bombCB();
//    void __bulletCB();
//    void __gravityCB();
//    void __rocketCB();
//    void __turretCB();

};

#endif // GAMECONFIGVIEW_H
