#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1600, 1000);

    h_tabs = new QTabWidget(this);
    h_tabs->setGeometry(100, 50, 1400, 900);
    h_tabs->setTabsClosable(true);
    connect(h_tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(removeSelectedTab(int)));

    h_btnImport = new QToolButton(this);
    h_btnImport->setText("Import");
    h_btnImport->setGeometry(0,0,50,20);
    connect(h_btnImport, SIGNAL(clicked()), SLOT(importNewImage()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::importNewImage()
{
    //QFileDialog *fd = new QFileDialog(this);
    //QString fileName = fd->getOpenFileName(this,tr("Open File"),"",tr("*.info"));
    QString fileName = "/home/wang/QDAS/image.info";
    if (fileName == "") return;
    QFileInfo info(fileName);
    QString filePath = info.absolutePath();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

//    QTextStream *inStream = new QTextStream(&file);
//    QString strImgNum = inStream->readLine();
//    h_imgNum = strImgNum.toInt();
//    h_img = new cv::Mat[h_imgNum];

//    for (int i = 0; i < h_imgNum; i++)
//    {
//        QString str = filePath + "/" + inStream->readLine();
//        h_img[i] = cv::imread(str.toLatin1().data(), 0);
//    }

//    cv::Mat h_channel[3];

//    h_channel[0] = h_img[2];
//    h_channel[1] = h_img[1];
//    h_channel[2] = h_img[0];

//    cv::merge(h_channel, 3, h_imgShow);
//    imgShow(h_imgShow);
//    h_isImport = 1;
//    h_isConvert = 0;
//    h_scale = 1;
}

void MainWindow::removeSelectedTab(int index)
{
    h_tabs->removeTab(index);
}
