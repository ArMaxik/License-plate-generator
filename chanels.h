#ifndef CHANELS_H
#define CHANELS_H

#include "nodes.h"

#include <QGraphicsObject>
#include <QLayout>

class BasicChanel : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QLayout *getSettingsLayout() = 0;
    void randomize();

public slots:
    void changeScale(qreal scale);
    void setBoundSize(QSizeF size);

signals:
    void sizeChanged();

protected:
//    QRectF bound;
    BoundRect *bound;
    bool enable;
    BasicNode *node;


};

class DiffuseChanel : public BasicChanel
{
    Q_OBJECT
public:
    DiffuseChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

//    void paint(QPainter *painter,
//               const QStyleOptionGraphicsItem *option,
//               QWidget *widget) override;

    QLayout *getSettingsLayout() override;
};

#endif // CHANELS_H
