#include "qimgfusion.h"

QImgFusion::QImgFusion()
{

}

QImgFusion::QImgFusion(QVector<QImgData*> images)
{
    srcImgs = images;
    activateFusionDialog();
}

QImgFusion::~QImgFusion()
{

}

void QImgFusion::activateFusionDialog()
{
    if (srcImgs.count() < 2)
    {
        QMessageBox::warning(NULL, "Warning", "Please import at least 2 images!", QMessageBox::Yes);
        return;
    }
    h_fusionDialog = new QDialog();
    h_fusionDialog->setWindowTitle("Image Fusion");
    h_fusionDialog->setFixedSize(300, 200);
    h_fusionDialog->show();

    QLabel *h_strMutiImage = new QLabel("MutiSpectral Image:", h_fusionDialog);
    h_strMutiImage->setGeometry(50, 20, 200, 20);
    h_strMutiImage->show();
    QLabel *h_strHighResoImage = new QLabel("High Resolution Image:", h_fusionDialog);
    h_strHighResoImage->setGeometry(50, 60, 200, 20);
    h_strHighResoImage->show();
    QLabel *h_strFusionType = new QLabel("Fusion type:", h_fusionDialog);
    h_strFusionType->setGeometry(50, 100, 200, 20);
    h_strFusionType->show();
    h_mutiImage = new QComboBox(h_fusionDialog);
    h_mutiImage->setGeometry(50, 40, 200, 20);
    h_mutiImage->show();
    h_highResoImage = new QComboBox(h_fusionDialog);
    h_highResoImage->setGeometry(50, 80, 200, 20);
    h_highResoImage->show();
    h_fusionType = new QComboBox(h_fusionDialog);
    h_fusionType->setGeometry(50, 120, 200, 20);
    h_fusionType->addItem("Non-weight");
    h_fusionType->addItem("Weight");
    h_fusionType->addItem("IHS");
    h_fusionType->show();
    for (int i = 0; i < srcImgs.count(); i++)
    {
        h_mutiImage->addItem(srcImgs[i]->imgName);
        h_highResoImage->addItem(srcImgs[i]->imgName);
    }

    QToolButton *h_ok = new QToolButton(h_fusionDialog);
    h_ok->setText("OK");
    h_ok->setGeometry(120, 150, 50, 20);
    connect(h_ok, SIGNAL(clicked()), this, SLOT(imgFusion()));
    h_ok->show();
}

void QImgFusion::imgFusion()
{
    h_fusionDialog->close();
    mutiImage = srcImgs[h_mutiImage->currentIndex()]->img;
    highResoImage = srcImgs[h_highResoImage->currentIndex()]->img;
    band = srcImgs[h_mutiImage->currentIndex()]->band;
    row = highResoImage->rows;
    col = highResoImage->cols;
    size = row * col;

    resImage = new cv::Mat[band];
    weight = new double[band];

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

    if (h_fusionType->currentIndex() != 2)
    {
        getWeight();
        for (int k = 0; k < band; k++)
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    double highResoVal = (double)highResoImage->at<unsigned char>(i, j);
                    double mutiVal = (double)resImage[k].at<unsigned char>(i, j);
                    resImage[k].at<unsigned char>(i, j) = ((1 + abs(weight[k])) * highResoVal + (1 - abs(weight[k])) * mutiVal)/2;
                }
            }
        }
    }

    if (h_fusionType->currentIndex() == 2)
    {
        IHSImage = new cv::Mat[3];
        RGB2IHS();
        histMatch();
        IHS2RGB();
    }

    emit emitNewImgData(resImage, band);
}

void QImgFusion::getWeight()
{
    if (h_fusionType->currentIndex() == 1)
    {
        for (int i = 0; i < band; i++)
        {
            weight[i] = 0;
        }
    }
    if (h_fusionType->currentIndex() == 2)
    {
        mutiMean = new double[band];
        highResoMean = new double;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                *highResoMean += (double)highResoImage->at<unsigned char>(i, j);
            }
        }

        for (int k = 0; k < band; k++)
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    mutiMean[k] += (double)resImage[k].at<unsigned char>(i, j);
                }
            }
        }
        *highResoMean = *highResoMean / size;
        for (int i = 0; i < band; i++)
        {
            mutiMean[i] = mutiMean[i] / size;
        }

        double foo = 0, bar = 0;
        double fooo = 0, barr = 0;
        for (int k = 0; k < band; k++)
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    foo = (double)highResoImage->at<unsigned char>(i, j) - *highResoMean;
                    bar = (double)resImage[k].at<unsigned char>(i, j) - mutiMean[k];
                    weight[k] += foo * bar;
                    fooo += foo * foo;
                    barr += bar * bar;
                }
            }
            weight[k] = weight[k] / sqrt(fooo + barr);
        }
    }
}

void QImgFusion::RGB2IHS()
{
    for (int i = 0; i < 3; i++)
    {
        IHSImage[i] = cv::Mat::zeros(row, col, CV_64FC1);
    }
    double r = 0, g = 0, b = 0;
    double I = 0, H = 0, S = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            b = (double)resImage[0].at<unsigned char>(i,j);
            g = (double)resImage[1].at<unsigned char>(i,j);
            r = (double)resImage[2].at<unsigned char>(i,j);

            I = (r + g + b) / sqrt(3.0);
            double m = r;
            if (g < m) m = g;
            if (b < m) m = b;
            S = (1 - sqrt(3.0) * m / I);
            double f = 0.5*(2*r-g-b)/sqrt((r-g)*(r-g)+(r-b)*(g-b));
            if (g >= b) H = acos(f);
            else H = 2 * PI - acos(f);

            IHSImage[0].at<double>(i, j) = I;
            IHSImage[1].at<double>(i, j) = H;
            IHSImage[2].at<double>(i, j) = S;
        }
    }
}

void QImgFusion::histMatch()
{
    double minHigh = 256;
    double minI = 10000000;
    double maxHigh = 0;
    double maxI = 0;
    for (int i = 0; i < row; i++)
    {
         for (int j = 0; j < col; j++)
        {
            double temp = (double)highResoImage->at<unsigned char>(i, j);
            if (temp > maxHigh) maxHigh = temp;
            if (temp < minHigh) minHigh = temp;
            double tempI = IHSImage[0].at<double>(i, j);
            if (tempI > maxI) maxI = tempI;
            if (tempI < minI) minI = tempI;
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            double temp = (double)highResoImage->at<unsigned char>(i, j);
            double val = (temp - minHigh) * (maxI - minI) / (maxHigh - minHigh) + minI;
            IHSImage[0].at<double>(i, j) = val;
        }
    }
}

void QImgFusion::IHS2RGB()
{
    double r = 0, g = 0, b = 0;
    double I = 0, H = 0, S = 0;
    double s = sqrt(3.0);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            I = IHSImage[0].at<double>(i, j);
            H = IHSImage[1].at<double>(i, j);
            S = IHSImage[2].at<double>(i, j);

            if (H >= 0 && H < 2 * PI / 3)
            {
                r=I*(1+S*cos(H)/cos(PI/3-H))/s;//R
                b=I*(1-S)/s;//G
                g=s*I-r-b;
            }
            else if (H >= 2 * PI / 3 && H < 4 * PI / 3)
            {
                g=I*(1+S*cos(H-2*PI/3)/cos(PI-H))/s;//G
                r=I*(1-S)/s;//R
                b=s*I-r-g;//B
            }
            else if (H >= 4 * PI / 3 && H < 2 * PI)
            {
                b=I*(1+S*cos(H-4*PI/3)/cos(5*PI/3-H))/s;//B
                g=I*(1-S)/s;//G
                r=s*I-b-g;//R
            }

            if (b < 0) b = 0;
            if (b > 255) b = 255;
            if (g < 0) g = 0;
            if (g > 255) g = 255;
            if (b < 0) b = 0;
            if (b > 255) b = 255;

            resImage[0].at<unsigned char>(i, j) = (unsigned char)b;
            resImage[1].at<unsigned char>(i, j) = (unsigned char)g;
            resImage[2].at<unsigned char>(i, j) = (unsigned char)r;
        }
    }
}
