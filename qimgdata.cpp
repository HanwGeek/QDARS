#include "qimgdata.h"

QImgData::QImgData(QWidget* parent)
{
    this->setParent(parent);
    imgView->setBackgroundBrush(QBrush(QColor(0,0,0)));

    connect(imgView, SIGNAL(emitWheel(double)), this, SLOT(wheelZoom(double)));
}

QImgData::QImgData(QString fileName)
{
    //this->setParent(parent);
    imgView = new QMouseView(this);
    imgView->setBackgroundBrush(QBrush(QColor(0,0,0)));
    imgScene = new QGraphicsScene(this);

    QFileInfo info(fileName);
    QString filePath = info.absolutePath();
    imgName = info.fileName();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QTextStream *inStream = new QTextStream(&file);
    band = inStream->readLine().toInt();
    pixelSize = inStream->readLine().toInt();
    upperLeftX = inStream->readLine().toDouble();
    upperLeftY = inStream->readLine().toDouble();
    lowerRightX = inStream->readLine().toDouble();
    lowerRightY = inStream->readLine().toDouble();

    imgView->pixelSize = pixelSize;
    imgView->upperLeftX = upperLeftX;
    imgView->upperLeftY = upperLeftY;
    imgView->lowerRightX = lowerRightX;
    imgView->lowerRightY = lowerRightY;

    img = new cv::Mat[band];
    for (int i = 0; i < band; i++)
    {
        QString str = filePath + "/" + inStream->readLine();
        img[i] = cv::imread(str.toLatin1().data(), 0);
    }

    if (band >= 3)
    {
        cv::Mat channel[3];

        channel[0] = img[2];
        channel[1] = img[1];
        channel[2] = img[0];

        cv::merge(channel, 3, imgShowInView);
    }
    if (band == 1)
    {
        imgShowInView = img[0];
    }
        imgView->resize(imgShowInView.cols, imgShowInView.rows);

    connect(imgView, SIGNAL(emitWheel(double)), this, SLOT(wheelZoom(double)));

    imgShow(imgShowInView);

    scale = 1;
    isConvert = 0;
}

QImgData::QImgData(cv::Mat *srcImg, int numBands)
{
    imgView = new QMouseView(this);
    imgView->setBackgroundBrush(QBrush(QColor(0,0,0)));
    imgScene = new QGraphicsScene(this);

    band = numBands;
    img = new cv::Mat[band];
    for (int i = 0; i < band; i++)
    {
        img[i] = srcImg[i];
    }

    if (band >= 3)
    {
        cv::Mat channel[3];

        channel[0] = img[0];
        channel[1] = img[1];
        channel[2] = img[2];

        cv::merge(channel, 3, imgShowInView);
    }
    if (band == 1)
    {
        imgShowInView = img[0];
    }

    imgName = "result image";
    pixelSize = 0;
    upperLeftX = upperLeftY = 0;
    lowerRightX = lowerRightY = 0;
    connect(imgView, SIGNAL(emitWheel(double)), this, SLOT(wheelZoom(double)));

    imgShow(imgShowInView);

    scale = 1;
    isConvert = 1;
}

QImgData::~QImgData()
{

}


QPointF QImgData::MapToImage(QPointF p)
{
    double x = (p.x() - upperLeftX) / pixelSize;
    double y = (upperLeftY - p.y()) / pixelSize;
    return QPointF(x, y);
}

QPointF QImgData::ImageToMap(QPointF p)
{
    int x = p.x() * pixelSize - upperLeftX;
    int y = upperLeftY - p.y() * pixelSize;
    return QPointF(x, y);
}

void QImgData::imgShow(cv::Mat image)
{
    if (isConvert == 0 && image.channels() == 3)
    {
        cv::cvtColor(image, image, CV_BGR2RGB);
    }

    isConvert = 1;
    imgScene->clear();
    if (band == 1) {
        imageShow = QImage((const unsigned char*)image.data, image.cols, image.rows, image.cols * image.channels(), QImage::Format_Grayscale8);
    }
    if (band >= 3)
    {
        imageShow = QImage((const unsigned char*)image.data, image.cols, image.rows, image.cols * image.channels(), QImage::Format_RGB888);
    }

    pixMap = QPixmap::fromImage(imageShow);
    pixMap = pixMap.scaled(imgView->width(), imgView->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imgScene->addPixmap(pixMap);
    imgView->setScene(imgScene);
    imgView->show();

}

void QImgData::wheelZoom(double d)
{
    if (d > 0) scale *= 1.2;
    if (d < 0) scale /= 1.2;
    double width = imgView->width()*scale;
    double height = imgView->height()*scale;
    imgScene->clear();
    pixMap = QPixmap::fromImage(imageShow);
    pixMap = pixMap.scaled(width, height, Qt::KeepAspectRatio);

    imgScene->setSceneRect(pixMap.rect());
    imgScene->addPixmap(pixMap);
}
