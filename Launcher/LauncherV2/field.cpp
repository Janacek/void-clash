#include "field.h"

Field::Field(QWidget *parent) : QWidget(parent)
{
    _gridLayout = new QGridLayout(this);
    _gridLayout->setHorizontalSpacing(0);
    _gridLayout->setVerticalSpacing(0);
    _gridLayout->setSpacing(0);
    _gridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(_gridLayout);
}

Field::~Field()
{

}

void    Field::init()
{
}

void    Field::addToLayout(QLayout *layout, int row, int column,
                           int rowSpan, int columnSpan)
{
    _gridLayout->addLayout(layout, row, column, rowSpan, columnSpan);
}

void    Field::addToLayout(QLayout *layout, int row, int column)
{
    _gridLayout->addLayout(layout, row, column);
}


void    Field::addToLayout(QWidget *widget, int row, int column,
                           int rowSpan, int columnSpan)
{
    _gridLayout->addWidget(widget, row, column, rowSpan, columnSpan);
}

void    Field::addToLayout(QWidget *widget, int row, int column)
{
    _gridLayout->addWidget(widget, row, column);
}

QGridLayout *Field::getGrid()
{
    return _gridLayout;
}

void Field::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }
