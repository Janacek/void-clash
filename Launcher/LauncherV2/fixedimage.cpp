#include "fixedimage.h"

FixedImage::FixedImage(QString &fileName, int height, int width, QWidget *parent):
    QLabel(parent)
{
    QPixmap pixmap(fileName);

    if (height != -1 && width != -1) {
        QSize size(height, width);
        pixmap.scaled(size, Qt::KeepAspectRatio);
    }
    setPixmap(pixmap);
}

FixedImage::~FixedImage()
{

}

