#ifndef GAMEBAR_H
#define GAMEBAR_H

#include "field.h"
#include "basicbutton.h"
#include "textbutton.h"
#include "common.h"
#include <QApplication>
#include <QLabel>
#include <QProcess>
#include <QDir>

class GameBar: public Field
{
    Q_OBJECT
public:
    GameBar(QWidget *parent = NULL);
    ~GameBar();

    virtual void init();
    void unselectedButtons();
    void selectGameConfig();
    void updateInfos(std::map<eInfoType, eSelectionType> &);

private:
    void _initLayouts();
    void _initTmpLayout();
    void _initQuickGameButton();
    void _initGameSettings();
    void _initGameMenu();

    void _updateMatchMode(eSelectionType);
//    void _updateWeapons(eSelectionType, eSelectionType);
    void _updateGameMode(eSelectionType);

    //Tmp Layout
    TextButton *_tmpLayout;

    //Launch match with settings
    BasicButton *_quickMatch;

    //Game Settings - Title
    QWidget     *_labelRankedTypeGrp;
    BasicButton *_starImage;
    QLabel      *_labelRankedType1;
    QLabel      *_labelRankedType2;

    //Game Settings - Choice
    QWidget     *_gameTypeChoiceGrp;
    BasicButton *_DMBtn;
    BasicButton *_FFABtn;
    BasicButton *_CTFBtn;
    BasicButton *_TSVRBtn;
    BasicButton *_HORBtn;
    BasicButton *_KOTHBtn;
    QLabel      *_labelRankedMode;

    //Game Settings - Weapon
//    QWidget     *_weaponsGrp;
//    QLabel      *_chosenWeapons;
//    QLabel      *_labelWeapon;

    //Change settings / message / parameters / close
    Field       *_gameMenu;
    BasicButton *_gameConfigBtn;
    BasicButton *_optionBtn;
    BasicButton *_quitBtn;


public slots:
    void __launchGame();
    void __quitGame();

};

#endif // GAMEBAR_H
