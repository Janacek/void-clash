#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QPushButton>
#include <QWidget>

class TextButton: public QPushButton
{
    Q_OBJECT

public:
    TextButton(QString text, QString fontName,
                int fontSize, QWidget *parent = 0);
    ~TextButton();

    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent( QEvent* e );

    bool isSelected() const;
    void setSelected(bool);
    void setStrStyle(const std::string &);
    void setStrHoverStyle(const std::string &);

    void switchSelection();

private:
    bool _selected;
    std::string _style;
    std::string _hoverStyle;

public slots:
    void __switchSelection();
};

#endif // TEXTBUTTON_H
