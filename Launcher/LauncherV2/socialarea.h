#ifndef SOCIALAREA_H
#define SOCIALAREA_H

#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include "field.h"

class SocialArea: public Field
{
    Q_OBJECT
public:
    SocialArea(QWidget *parent = NULL);
    ~SocialArea();

    virtual void init();
    void paintEvent(QPaintEvent *);
};

#endif // SOCIALAREA_H
