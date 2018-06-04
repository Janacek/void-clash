#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "field.h"
#include "toolsbar.h"
#include "gamebar.h"
#include "choiceview.h"
#include "socialarea.h"
#include "refreshinfosevent.h"

#include "common.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void start();
    void setGeneralInfo(eInfoType, eSelectionType);

    bool event(QEvent *event);
private:
    void            _initGeneralInfos();
    void            _initLayouts();

    Field           *_mainField;
    ToolsBar        *_toolsBar;
    GameBar         *_gameBar;
    ChoiceView      *_choiceView;
    SocialArea      *_socialArea;

    std::map<eInfoType, eSelectionType> _generalInfos;

public slots:
    void __activateWeapons();
    void __activateServerList();
    void __activateNews();
};

#endif // MAINWINDOW_H
