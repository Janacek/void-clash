#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>

class Field: public QWidget
{
    Q_OBJECT
public:
    explicit Field(QWidget *parent = 0);
    ~Field();
    void paintEvent(QPaintEvent *);

    virtual void init();

    QGridLayout    *getGrid();

    void    addToLayout(QLayout *, int, int, int, int);
    void    addToLayout(QLayout *, int, int);
    void    addToLayout(QWidget *, int, int, int, int);
    void    addToLayout(QWidget *, int, int);
protected:
    QGridLayout *_gridLayout;
};

#endif // FIELD_H
