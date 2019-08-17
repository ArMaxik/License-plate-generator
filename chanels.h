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

    QLayout *getSettingsLayout();
    void randomize();

    enum Nodes { ImageN, TextN, ShapeN };
public slots:
    void changeScale(qreal scale);
    void setBoundSize(QSizeF size);

signals:
    void sizeChanged();

protected:
//    QRectF bound;
    QVector<Nodes> allowedNodes;
    Nodes currentNode;
    BoundRect *bound;
    bool enable;
    BasicNode *node;

protected slots:
    void onNodeChange(int index);
};

class DiffuseChanel : public BasicChanel
{
    Q_OBJECT
public:
    DiffuseChanel(BoundRect *br, QGraphicsItem *parent = nullptr);
};

class SpecularChanel : public BasicChanel
{
    Q_OBJECT
public:
    SpecularChanel(BoundRect *br, QGraphicsItem *parent = nullptr);
};

#endif // CHANELS_H
