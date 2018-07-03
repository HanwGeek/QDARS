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
