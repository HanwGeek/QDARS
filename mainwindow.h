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
#include <math.h>
#include <QDebug>

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
    void creatNewTab(cv::Mat *img, int numBands);
    //void bcastImages(QVector<QImgData*> *images);
private:
    QTabWidget* h_tabs;
    QToolButton *h_btnImport;
    QToolButton *h_btnImgSave;
    QToolButton *h_btnFusion;


    QImgFusion *h_imgFusion;

    int h_imgNum;
    int h_preIndex;
    double *h_weight;
};

#endif // MAINWINDOW_H
