#include "qkmeans.h"

const int colorMap[5][3] = {
    {205, 197, 191},//seashell
    {238, 118, 0},//darkorange2
    {50, 205, 50},//limegreen
    {135, 206, 250},//lightskyblue
    {168, 30, 199}//purple
};

QKmeans::QKmeans(QObject *parent) : QObject(parent)
{

}

QKmeans::~QKmeans()
{

}

QKmeans::QKmeans(QImgData *image)
{
    img = image->img;
    band = image->band;
    row = img[0].rows;
    col = img[0].cols;


    typeNum = 5;
    loopNum = 25;
    center = new int*[typeNum];
    oldCenter = new int*[typeNum];
    count = new int[typeNum];
    for (int i = 0; i < typeNum; i++)
    {
        center[i] = new int[band];
        oldCenter[i] = new int[band];
        count[i] = 0;
    }
    feature = new cv::Mat(row, col, CV_8UC1);
    resImg = new cv::Mat[band];
    for (int i = 0; i < band; i++)
    {
        resImg[i] = cv::Mat(row, col, CV_8UC1);
    }
}
void QKmeans::divide()
{
    initCenter();
    for (int k = 0; k < loopNum; k++)
    {

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                updateSample(i, j);
            }
        }
        updateCenter();
        if (check() == 1) break;
    }
    plot();
//    cv::imshow("img", *resImg);
//    cv::waitKey(0);
    emit emitNewImgData(resImg, 3);
}

void QKmeans::initCenter()
{
    srand((unsigned int)time(NULL));

    for (int i = 0; i < typeNum; i++)
    {
        for (int j = 0; j < band; j++)
        {
            center[i][j] = rand() % 80 + 40;
        }
        count[i] = 0;
    }
}

void QKmeans::updateSample(int x, int y)
{
    unsigned char *s = new unsigned char[band];
    for (int i = 0; i < band; i++)
    {
        s[i] = img[i].at<unsigned char>(x, y);
    }
    unsigned char idx = 0;
    int min = INT_MAX;
    for (int i = 0; i <typeNum; i++)
    {
        int  distance = 0;
        if (count[i] != -1)
        {
            for (int j = 0; j < band; j++)
            {
                distance += (center[i][j] - (int)s[j]) * (center[i][j] - (int)s[j]);
            }
            if (distance < min)
            {
                min = distance;
                idx = i;
            }
        }
    }
    feature->at<unsigned char>(x, y) = idx;
    count[idx]++;
}

void QKmeans::updateCenter()
{

    for (int i = 0; i < typeNum; i++)
    {
        if (count[i] != 0)
        {
            for (int j = 0; j < band; j++)
            {
                oldCenter[i][j] = center[i][j];
                center[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            unsigned char idx = feature->at<unsigned char>(i, j);
            for (int k = 0; k < band; k++)
            {
                center[idx][k] += (int)img[k].at<unsigned char>(i, j);
            }
        }
    }

    for (int i = 0; i < typeNum; i++)
    {
        for (int j = 0; j < band; j++)
        {
            if (count[i] != 0)
            {
                center[i][j] = center[i][j] / count[i];
            }
        }
    }
    for (int i = 0; i < typeNum; i++)
    {
        count[i] = 0;
    }
}

int QKmeans::check()
{
    for (int i = 0; i < typeNum; i++)
    {
        for (int j = 0; j < band; j++)
        {
            int eps = center[i][j] - oldCenter[i][j];
            if (eps < -1 || eps > 1)
            {
                return 0;
            }
        }
    }
    return 1;
}

void QKmeans::plot()
{
    for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                unsigned char idx = feature->at<unsigned char>(i, j);
                resImg[0].at<unsigned char>(i, j) = colorMap[idx][0];
                resImg[1].at<unsigned char>(i, j) = colorMap[idx][1];
                resImg[2].at<unsigned char>(i, j) = colorMap[idx][2];
            }
        }
}


