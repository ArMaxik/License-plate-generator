#ifndef BOUNDRECT_H
#define BOUNDRECT_H

#include "QRectF"

#include <QObject>

class BasicItem;

class BoundRect : public QObject {
    Q_OBJECT
public:
    BoundRect(QRectF rect, BasicItem *bi);

    QRectF getBound() const;
    qreal getScale() const;
//    qreal diffuseChanelScale() const  { return dScale; }
//    qreal specularChanelScale() const { return sScale; }

public slots:
    void setSize(QSizeF size);
    void setScale(qreal scale);

signals:
    void sizeChanged(QRectF bound);

private:
    QRectF bound;
    BasicItem *item;
//    qreal dScale;
//    qreal sScale;
};

#endif // BOUNDRECT_H
