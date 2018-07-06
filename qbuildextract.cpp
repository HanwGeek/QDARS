#include "qbuildextract.h"

QBuildExtract::QBuildExtract()
{

}

QBuildExtract::~QBuildExtract()
{

}

QBuildExtract::QBuildExtract(QImgData *Image)
{
    img = Image->img;
    band = Image->band;
    row = img[0].rows;
    col = img[0].cols;
    resImg = new cv::Mat(row, col, CV_8UC1);
}

void QBuildExtract::extract()
{
    double SNDBI = 0, NDWI = 0, SAVI = 0, MIBI = 0;
    double blue = 0, green = 0, red = 0, NIR = 0;
    alpha = 0;
    beta = -1;
    thr = 5;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            blue = (double)img[0].at<unsigned char>(i, j);
            green = (double)img[1].at<unsigned char>(i, j);
            red = (double)img[2].at<unsigned char>(i, j);
            NIR = (double)img[3].at<unsigned char>(i, j);

            NDWI =(green - NIR) / (green + NIR);
            SNDBI = (NIR - green) / (NIR + green);
            if (SNDBI <= beta) SNDBI = 1.0;
            SAVI = (NIR - red)*(1 + alpha) / (NIR + red + alpha);
            double _t = (NDWI + SAVI) / 2;
            MIBI = (SNDBI - _t) / (SNDBI + _t);
            if (MIBI < 1.9) resImg->at<unsigned char>(i, j) = 0;
            else resImg->at<unsigned char>(i, j) = 255;
            //resImg->at<unsigned char>(i, j) = (unsigned char)(MIBI * 255);
        }
    }

    emit emitNewImgData(resImg, 1);
}
