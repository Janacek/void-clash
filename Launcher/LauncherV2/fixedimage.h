#ifndef FIXEDIMAGE_H
#define FIXEDIMAGE_H

#include <QLabel>
#include <QPixmap>
#include <QIcon>

class FixedImage: QLabel
{
public:
    FixedImage(QString &fileName, int height = -1, int width = -1, QWidget *parent = 0);
    ~FixedImage();
};

#endif // FIXEDIMAGE_H
