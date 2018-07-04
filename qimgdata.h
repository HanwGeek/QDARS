#ifndef QIMGDATA_H
#define QIMGDATA_H

#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QObject>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <qmouseview.h>
#include <QGraphicsScene>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QRect>
#include <QPointF>
#include <QImage>
#include <QPixmap>

class QImgData:public QWidget
{
    Q_OBJECT
public:
    QImgData(QWidget* parent = 0);
    QImgData(QString fileName);
    ~QImgData();
    
    void imgShow(cv::Mat img);
    QRect MapToImage(QRect Map);
    QRect ImageToMap(QRect Image);
    QPointF MapToImage(QPointF p);
    QPointF ImageToMap(QPointF p);

    QMouseView *imgView;
    QGraphicsScene *imgScene;
    QImage imageShow;
    QPixmap pixMap;
    QString imgName;

    cv::Mat* img;
    cv::Mat imgShowInView;
    int band;
    int pixelSize;
    double upperLeftX;
    double upperLeftY;
    double lowerRightX;
    double lowerRightY;
    double scale;

    int isConvert;
public slots:
    void wheelZoom(double d);

};

#endif // QIMGDATA_H
