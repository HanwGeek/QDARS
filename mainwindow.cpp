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
    connect(h_btnFusion, SIGNAL(clicked()), SLOT(activateFusionDialog()));

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


void MainWindow::activateFusionDialog()
{
    if (h_images.count() < 2)
    {
        QMessageBox::warning(NULL, "Warning", "Please import at least 2 images!", QMessageBox::Yes);
        return;
    }
    h_fusionDialog = new QDialog();
    h_fusionDialog->setWindowTitle("Image Fusion");
    h_fusionDialog->resize(300, 200);
    h_fusionDialog->show();

    QLabel *h_strMutiImage = new QLabel("MutiSpectral Image:", h_fusionDialog);
    h_strMutiImage->setGeometry(50, 20, 200, 20);
    h_strMutiImage->show();
    QLabel *h_strHighResoImage = new QLabel("High Resolution Image:", h_fusionDialog);
    h_strHighResoImage->setGeometry(50, 60, 200, 20);
    h_strHighResoImage->show();
    h_mutiImage = new QComboBox(h_fusionDialog);
    h_mutiImage->setGeometry(50, 40, 200, 20);
    h_mutiImage->show();
    h_highResoImage = new QComboBox(h_fusionDialog);
    h_highResoImage->setGeometry(50, 80, 200, 20);
    h_highResoImage->show();
    for (int i = 0; i < h_images.count(); i++)
    {
        h_mutiImage->addItem(h_images[i]->imgName);
        h_highResoImage->addItem(h_images[i]->imgName);
    }

    QToolButton *h_ok = new QToolButton(h_fusionDialog);
    h_ok->setText("OK");
    h_ok->setGeometry(120, 120, 50, 20);
    connect(h_ok, SIGNAL(clicked()), this, SLOT(imgFusion()));
    h_ok->show();
}

void MainWindow::imgFusion()
{
    h_fusionDialog->close();
    cv::Mat *mutiImage = h_images[h_mutiImage->currentIndex()]->img;
    cv::Mat *highResoImage = h_images[h_highResoImage->currentIndex()]->img;
    int band = mutiImage[0].channels();

    cv::Mat resImage[band];
    for (int i = 0; i < band; i++)
    {
        resImage[i] = cv::Mat::zeros(highResoImage[0].rows, highResoImage[0].cols, CV_8UC1);
    }

    if (mutiImage[0].rows != highResoImage[0].rows)
    {
        for (int i = 0; i < band; i++)
        {
            cv::resize(mutiImage[i], resImage[i], resImage[i].size());
        }
    }
    else {
        for (int i = 0; i < band; i++)
        {
            resImage[i] = mutiImage[i];
        }
    }

    qDebug() << resImage[0].rows;
}




















