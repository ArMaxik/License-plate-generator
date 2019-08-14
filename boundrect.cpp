#include "boundrect.h"
#include "basicitem.h"

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
    item->setScale(1.0);
    bound.setSize(size);

    emit sizeChanged(QRectF(0.0, 0.0,
                            bound.width()*item->scale(),
                            bound.height()*item->scale()));
}

void BoundRect::setScale(qreal scale)
{
//    setSize(bound.size() / getScale() * scale);
    item->setScale(scale);
    emit sizeChanged(QRectF(0.0, 0.0,
                            bound.width()*item->scale(),
                            bound.height()*item->scale()));
}
