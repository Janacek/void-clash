#include "basicbutton.h"

BasicButton::BasicButton(QString picName, QString hoverPicName,
                         int height, int width, QWidget *parent)
    : QPushButton("", parent)
{
    setFont(QFont("Arial", 19, QFont::Bold));


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
    setStyleSheet("background:rgba(30, 30, 30, 0)");
    _selected = false;
}

BasicButton::~BasicButton()
{
    delete _normal;
    delete _onHover;
}

bool BasicButton::isSelected() const
{
    return _selected;
}

void BasicButton::setSelected(bool selected)
{
    if (!selected)
        setIcon(*_normal);
    else
        setIcon(*_onHover);
    _selected = selected;
}

void BasicButton::enterEvent( QEvent* e )
{
    setIcon(*_onHover);
    QWidget::enterEvent( e );
}

void BasicButton::leaveEvent( QEvent* e )
{
    if (!_selected)
        setIcon(*_normal);
    QWidget::leaveEvent( e );
}

void BasicButton::switchSelection()
{
    _selected ^= true;
}

//PUBLIC SLOTS (Signal Buttons)
void BasicButton::__switchSelection()
{
    _selected ^= true;
}
