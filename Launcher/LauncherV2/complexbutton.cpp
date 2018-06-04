#include "complexbutton.h"

#include <QFont>

ComplexButton::ComplexButton(QString text, QString fontName, int fontSize, int tPosX, int tPosY,
                             QString picName, QString hoverPicName, int height, int width, int bPosX, int bPosY,
                             QWidget *parent)
    : QPushButton(text, parent)
{
    QFont font(fontName);
    font.setPointSize(fontSize);
    setFont(font);
    setStyleSheet("color : rgb(206, 206, 214);"
                  "background-color:rgb(40, 40, 40, 0);");
    _selected = false;

    QPixmap pixmap(picName);
    QPixmap pixmapHover(hoverPicName);

    _normal = new QIcon(pixmap);
    _onHover = new QIcon(pixmapHover);
    setIcon(*_normal);

    if (height == -1 || width == -1) {
        setIconSize(pixmap.size());
    } else {
        QSize iconSize(height, width);
        setIconSize(iconSize);
    }
    adjustSize();
}

ComplexButton::~ComplexButton()
{
    delete _normal;
    delete _onHover;
}

bool ComplexButton::isSelected() const
{
    return _selected;
}

void ComplexButton::setSelected(bool selected)
{
    if (!selected) {
        setStyleSheet("color : rgb(206, 206, 214);"
                                        "background-color:rgb(40, 40, 40, 0);");
        setIcon(*_normal);
    }
    else {
        setStyleSheet("color : rgb(165, 192, 35);"
                      "background-color:rgb(40, 40, 40, 0);");
        setIcon(*_onHover);
    }
    _selected = selected;
}

void ComplexButton::enterEvent( QEvent* e )
{
    setIcon(*_onHover);
    setStyleSheet("color : rgb(165, 192, 35);"
                  "background-color:rgb(40, 40, 40, 0);");
    QWidget::enterEvent( e );
}

void ComplexButton::leaveEvent( QEvent* e )
{
    if (!_selected) {
        setStyleSheet("color : rgb(206, 206, 214);"
                                        "background-color:rgb(40, 40, 40, 0);");
        setIcon(*_normal);
    }
    QWidget::leaveEvent( e );
}

void ComplexButton::switchSelection()
{
    _selected ^= true;
}

//PUBLIC SLOTS (Signal Buttons)
void ComplexButton::__switchSelection()
{
    _selected ^= true;
}
