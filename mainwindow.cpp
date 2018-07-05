#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1600, 1000);

    h_tabs = new QTabWidget(this);
    h_tabs->setGeometry(100, 50, 1400, 900);
    h_tabs->setTabsClosable(true);
    connect(h_tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(removeSelectedTab(int)));
    connect(h_tabs, SIGNAL(currentChanged(int)), this, SLOT(changeViewConnects(int)));
    h_tabs->hide();

    h_btnImport = new QToolButton(this);
    h_btnImport->setText("Import");
    h_btnImport->setGeometry(0,0,50,20);
    connect(h_btnImport, SIGNAL(clicked()), SLOT(importNewImage()));

    h_btnFusion = new QToolButton(this);
    h_btnFusion->setText("Fusion");
    h_btnFusion->setGeometry(50, 0, 50, 20);
    connect(h_btnFusion, SIGNAL(clicked()), this, SLOT(activateFusion()));
    h_preIndex = 0;
}

MainWindow::~MainWindow()
{

}

void MainWindow::importNewImage()
{

    QFileDialog *fd = new QFileDialog(this);
    QString fileName = fd->getOpenFileName(this,tr("Open File"),"",tr("*.info"));
    //QString fileName = "home/wang/QDAS/spot.info";
    if (fileName == "") return;

    if (h_tabs->isHidden() == true)
    {
        h_tabs->show();
    }
    QImgData *newImage = new QImgData(fileName);

    h_images.append(newImage);
    h_tabs->addTab(newImage->imgView, newImage->imgName);

    if (h_preIndex == 0)
    {
        connect(h_images[h_preIndex]->imgView, SIGNAL(emitNowPos(QPointF)), this, SLOT(updateStatusBar(QPointF)));
    }
}

void MainWindow::removeSelectedTab(int index)
{
    h_tabs->removeTab(index);
    h_images.remove(index);
    if (h_images.count() == 0) return;
    if (index == h_tabs->currentIndex())
    {
        connect(h_images[index]->imgView, SIGNAL(emitNowPos(QPointF)), this, SLOT(updateStatusBar(QPointF)));
    }
}

void MainWindow::changeViewConnects(int index)
{
    disconnect(h_images[h_preIndex]->imgView, SIGNAL(emitNowPos(QPointF)), this, SLOT(updateStatusBar(QPointF)));
    connect(h_images[index]->imgView, SIGNAL(emitNowPos(QPointF)), this, SLOT(updateStatusBar(QPointF)));
    h_preIndex = index;
}

void MainWindow::updateStatusBar(QPointF p)
{
    QString msg = QString::number(p.x(), 'f', 1) + ", " +QString::number(p.y(), 'f', 1);
    statusBar()->showMessage(msg);
}

void MainWindow::activateFusion()
{
    h_imgFusion = new QImgFusion(h_images);
    connect(h_imgFusion, SIGNAL(emitNewImgData(cv::Mat*,int)), this, SLOT(creatNewTab(cv::Mat*,int)));
}

void MainWindow::creatNewTab(cv::Mat *img, int numBands)
{
    QImgData *newImage = new QImgData(img, numBands);
    h_images.append(newImage);
    h_tabs->addTab(newImage->imgView, newImage->imgName);

    if (h_preIndex == 0)
    {
        connect(h_images[h_preIndex]->imgView, SIGNAL(emitNowPos(QPointF)), this, SLOT(updateStatusBar(QPointF)));
    }
}


















