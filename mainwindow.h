#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QImage>
#include <QFile>
#include <QLabel>
#include <QWidget>
#include <QTextStream>
#include <QFileInfo>
#include <QBrush>
#include <QColor>
#include <QToolButton>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <qimgdata.h>
#include <qimgfusion.h>
#include <qkmeans.h>
#include <qbuildextract.h>
#include <math.h>
#include <QDebug>
#include <QToolBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<QImgData*> h_images;
public slots:
    void removeSelectedTab(int index);
    void importNewImage();
    void changeViewConnects(int index);
    void updateStatusBar(QPointF p);
    void activateFusion();
    void activateBuildExtract();
    void activateKmeans();
    void creatNewTab(cv::Mat *img, int numBands);
    //void bcastImages(QVector<QImgData*> *images);
private:
    QTabWidget* h_tabs;
    QToolBox *h_toolbox;
    QToolButton *h_btnImport;
    QToolButton *h_btnImgSave;
    QToolButton *h_btnFusion;
    QToolButton *h_btnBuildExtract;
    QToolButton *h_btnKmeans;

    QImgFusion *h_imgFusion;
    QBuildExtract *h_imgBuildExtract;
    QKmeans *h_kmeans;
    int h_imgNum;
    int h_preIndex;
    double *h_weight;
};

#endif // MAINWINDOW_H
