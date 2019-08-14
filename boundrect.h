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

public slots:
    void setSize(QSizeF size);
    void setScale(qreal scale);

signals:
    void sizeChanged(QRectF bound);

private:
    QRectF bound;
    BasicItem *item;
};

#endif // BOUNDRECT_H
