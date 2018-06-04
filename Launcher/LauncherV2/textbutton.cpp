#include "textbutton.h"

TextButton::TextButton(QString text, QString fontName, int fontSize, QWidget *parent)
: QPushButton(text, parent)
{
    QFont font(fontName);
    font.setPointSize(fontSize);
    setFont(font);
    _style = "color : rgb(206, 206, 214);background-color:rgb(40, 40, 40, 0);";
    _hoverStyle = "color : rgb(165, 192, 35);background-color:rgb(40, 40, 40, 0);";
    setStyleSheet(_style.c_str());
    _selected = false;
}

TextButton::~TextButton()
{

}

bool TextButton::isSelected() const
{
    return _selected;
}

void TextButton::setSelected(bool selected)
{
    if (!selected)
        setStyleSheet(_style.c_str());
    else
        setStyleSheet(_hoverStyle.c_str());
    _selected = selected;
}

void    TextButton::setStrStyle(const std::string &style)
{
    _style = style;
    setStyleSheet(_style.c_str());
}

void    TextButton::setStrHoverStyle(const std::string &hoverStyle)
{
    _hoverStyle = hoverStyle;
}

void TextButton::enterEvent( QEvent* e )
{
    setStyleSheet(_hoverStyle.c_str());
    QWidget::enterEvent( e );
}

void TextButton::leaveEvent( QEvent* e )
{
    if (!_selected)
        setStyleSheet(_style.c_str());
    QWidget::leaveEvent(e);
}

void TextButton::switchSelection()
{
    _selected ^= true;
}

//PUBLIC SLOTS (Signal Buttons)
void TextButton::__switchSelection()
{
    _selected ^= true;
}
