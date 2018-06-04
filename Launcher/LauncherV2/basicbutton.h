#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QString>

class BasicButton: public QPushButton
{
    Q_OBJECT

public:
    BasicButton(QString picName, QString hoverPicName,
                int height, int width, QWidget *parent = NULL);
    ~BasicButton();

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

#endif // BASICBUTTON_H
