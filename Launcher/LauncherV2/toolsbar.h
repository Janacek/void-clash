#ifndef TOOLSBAR_H
#define TOOLSBAR_H

#include "field.h"
#include "basicbutton.h"
#include "textbutton.h"
#include <QProcess>
#include <QDir>

class ToolsBar: public Field
{
    Q_OBJECT
public:
    ToolsBar(QWidget *parent = NULL);
    ~ToolsBar();

    virtual void init();
    void unselectedButtons();
    void selectServerList();
    void selectNews();

private:
    void    _initLayouts();
    void    _initRightButtons();
    void    _initLeftButtons();

    //Right Buttons
    Field   *_rightButtons;
    BasicButton *_newsBtn;

    //Left Buttons
    QWidget   *_leftButtons;
    BasicButton *_serverListButton;
    BasicButton *_mapEditorButton;

public slots:
    void __launchMapEditor();
};

#endif // TOOLSBAR_H
