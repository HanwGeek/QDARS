#ifndef QIMGFUSION_H
#define QIMGFUSION_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <math.h>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QToolButton>
#include <QObject>
#include <QMessageBox>
#include <QVector>
#include <QString>
#include <qimgdata.h>

#ifndef PI
#define PI 3.1415926535
#endif

class QImgFusion: public QObject
{ Q_OBJECT
public:
    QImgFusion();
    QImgFusion(QVector<QImgData*> images);
    ~QImgFusion();

    QDialog *h_fusionDialog;
    QComboBox *h_mutiImage;
    QComboBox *h_highResoImage;
    QComboBox *h_fusionType;
    QVector<QImgData*> srcImgs;

    double *highResoMean;
    double *mutiMean;
    double *weight;
    double *Intensity;
    double *Hue;
    double *Saturation;
    int band;
    int size;
    int row;
    int col;

    cv::Mat *mutiImage;
    cv::Mat *highResoImage;
    cv::Mat *resImage;
    cv::Mat *IHSImage;

    void getWeight();
    void IHS2RGB();
    void RGB2IHS();
    void histMatch();
    void activateFusionDialog();
public slots:
    void imgFusion();

signals:
    void emitNewImgData(cv::Mat *img, int numBands);
};

#endif // QIMGFUSION_H
