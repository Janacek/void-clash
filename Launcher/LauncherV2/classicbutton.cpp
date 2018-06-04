#include "classicbutton.h"

ClassicButton::ClassicButton(QString &name)
    : QPushButton(name, NULL)
{
    setFont(QFont("Arial", 19, QFont::Bold));
    setStyleSheet("background-color:rgb(0, 55, 208);"
                  "border: 2px solid black;"
                  "selection-color: rgb(179, 190, 219);"
                  "selection-background-color: rgb(179, 190, 219);"
                  "height: 100%;width: 100%;"
                  "margin:0px;padding:0px;");
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(sizePolicy.hasHeightForWidth());
    setSizePolicy(sizePolicy);
}

ClassicButton::~ClassicButton()
{

}
