#ifndef QKMEANS_H
#define QKMEANS_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

class QKmeans : public QObject
{
    Q_OBJECT
public:
    explicit QKmeans(QObject *parent = nullptr);
    ~QKmeans();

signals:

public slots:
};

#endif // QKMEANS_H
