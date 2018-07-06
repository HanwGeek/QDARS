#ifndef QBUILDEXTRACT_H
#define QBUILDEXTRACT_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <qimgdata.h>
#include <QObject>

class QBuildExtract:public QObject
{   Q_OBJECT
public:
    QBuildExtract();
    QBuildExtract(QImgData *Image);
    ~QBuildExtract();

    cv::Mat *img;
    cv::Mat *resImg;
    cv::Mat NDWIImg;
    cv::Mat SAVIImg;

    int row;
    int col;
    int band;
    double alpha;
    double beta;
    double thr;

    void extract();

signals:
    void emitNewImgData(cv::Mat *img, int numBands);
};

#endif // QBUILDEXTRACT_H
