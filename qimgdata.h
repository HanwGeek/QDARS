#ifndef QIMGDATA_H
#define QIMGDATA_H

#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QObject>
#include <qmouseview.h>
#include <QGraphicsScene>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QRect>

class QImgData:public QWidget
{
    Q_OBJECT
public:
    QImgData(QWidget* parent = 0);
    ~QImgData();
    

    QMouseView *imgView;
    QGraphicsScene *scene;
    QRect MapToImage(QRect Map);
    QRect ImageToMap(QRect Image);

    cv::Mat h_img;

    int band;
    int pixelSize;
    double upperLeftX;
    double upperLeftY;
    double lowerRightX;
    double lowerRightY;
};

#endif // QIMGDATA_H
