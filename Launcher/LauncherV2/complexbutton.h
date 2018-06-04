#ifndef COMPLEXBUTTON_H
#define COMPLEXBUTTON_H

#include <QWidget>
#include <QPushButton>

class ComplexButton: public QPushButton
{
    Q_OBJECT
public:
    ComplexButton(QString , QString , int, int, int,
                  QString , QString , int, int, int, int
                  , QWidget *parent = NULL);
    ~ComplexButton();

    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent( QEvent* e );

    bool isSelected() const;
    void setSelected(bool);


    void switchSelection();
private:
    QIcon *_normal;
    QIcon *_onHover;

    bool _selected;

public slots:
    void __switchSelection();
};

#endif // COMPLEXBUTTON_H
