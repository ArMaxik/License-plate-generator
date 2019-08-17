#include "boundrect.h"
#include "basicitem.h"

#include <QDebug>

BoundRect::BoundRect(QRectF rect, BasicItem *bi)
    : bound(rect)
    , item(bi)
{

}

QRectF BoundRect::getBound() const
{
    return bound;
}

qreal BoundRect::getScale() const
{
    return item->scale();
}

void BoundRect::setSize(QSizeF size)
{
    item->prepareGeometryChange();
    bound.setSize(size);

    emit sizeChanged(QRectF(0.0, 0.0,
                            bound.width(),
                            bound.height()));
}

void BoundRect::setScale(qreal scale)  // Useless
{
//    setSize(bound.size() / getScale() * scale);
    item->setScale(scale);
    emit sizeChanged(QRectF(0.0, 0.0,
                            bound.width()*item->scale(),
                            bound.height()*item->scale()));
}
