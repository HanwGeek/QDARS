#include "qmouseview.h"

QMouseView::QMouseView(QWidget* parent)
{
    this->setParent(parent);
    setMouseTracking(true);
}

QMouseView::~QMouseView()
{

}

void QMouseView::mouseMoveEvent(QMouseEvent *e)
{
    emit emitNowPos(ImageToMap(mapToScene(e->x(), e->y())));
}

void QMouseView::mousePressEvent(QMouseEvent *e)
{

}

void QMouseView::wheelEvent(QWheelEvent *e)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        emit emitWheel(e->delta());
    }
}

QPointF QMouseView::MapToImage(QPointF p)
{
    double x = (p.x() - upperLeftX) / pixelSize;
    double y = (upperLeftY - p.y()) / pixelSize;
    return QPointF(x, y);
}

QPointF QMouseView::ImageToMap(QPointF p)
{
    int x = p.x() * pixelSize + upperLeftX;
    int y = upperLeftY - p.y() * pixelSize;
    return QPointF(x, y);
}

//void QMouseView::paintEvent(QPaintEvent *e)
//{

//}













