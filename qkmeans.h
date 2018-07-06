#ifndef QKMEANS_H
#define QKMEANS_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <qimgdata.h>
#include <time.h>

class QKmeans : public QObject
{
    Q_OBJECT
public:
    explicit QKmeans(QObject *parent = nullptr);
    QKmeans(QImgData *image);
    ~QKmeans();
    cv::Mat *img;
    cv::Mat *resImg;
    cv::Mat *feature;

    int row;
    int col;
    int band;
    int loopNum;
    int typeNum;
    //int colorMap[5][3];
    int **center;
    int **oldCenter;
    int *count;

    void divide();
    void initCenter();
    void updateSample(int x, int y);
    void updateCenter();
    int check();
    void plot();

signals:
    void emitNewImgData(cv::Mat *img, int numBands);
public slots:
};

#endif // QKMEANS_H
