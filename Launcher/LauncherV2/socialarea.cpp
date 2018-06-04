#include "socialarea.h"

#include <QLabel>
#include <QPixmap>
#include <iostream>
#include <QSize>

SocialArea::SocialArea(QWidget *parent): Field(parent)
{

}

SocialArea::~SocialArea()
{

}

void SocialArea::init()
{
    setFixedSize(640, 600);
    setStyleSheet("background-color:rgb(0, 0, 0, 140)");


    QPixmap image("../assets/launcher/tuto.png");
    QLabel *imageLabel = new QLabel(this);
    imageLabel->setPixmap(image);
    imageLabel->move(0, 70);
    imageLabel->setFixedWidth(640);
    imageLabel->setFixedHeight(360);
    imageLabel->setScaledContents(true);
    imageLabel->show();
}

//----------------
// PRIVATE FUNCTIONS
//----------------

void SocialArea::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }
