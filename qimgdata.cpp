#include "qimgdata.h"

QImgData::QImgData(QWidget* parent)
{
    this->setParent(parent);
    imgView->setBackgroundBrush(QBrush(QColor(0,0,0)));

    QObject::connect(imgView, SIGNAL(emitWheel(double)), this, SLOT(wheelZoom(double)));

}

QImgData::~QImgData()
{

}

QRect QImgData::MapToImage(QRect Map)
{
    int x = (Map.x() - upperLeftX) / pixelSize;
    int y = (upperLeftY - Map.y()) / pixelSize;
    int width = Map.width() / pixelSize;
    int height = Map.height() / pixelSize;
    QRect Image(x, y, width, height);
    return Image;
}

QRect QImgData::ImageToMap(QRect Image)
{
    int x = Image.x() * pixelSize + upperLeftX;
    int y = upperLeftY - Image.y() * pixelSize;
    int width = Image.width() * pixelSize;
    int height = Image.height() * pixelSize;
    QRect Map(x, y, width, height);
    return Map;
}
