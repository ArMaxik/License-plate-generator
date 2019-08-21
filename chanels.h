#ifndef CHANELS_H
#define CHANELS_H

#include "nodes.h"

#include <QGraphicsObject>
#include <QLayout>
#include <QStack>

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

    enum Nodes { ImageN, TextN, ShapeN, FillBackN, ImageBackN };

public slots:
    void changeScale(qreal scale)   { bound->setScale(scale); }
    void setBoundSize(QSizeF size)  { bound->setSize(size); }
    void setAffectSize(bool affect) { affectSize = affect; }
    void addAllowedNode(Nodes node) { allowedNodes.push_back(node); }
    void setNode(int index);

signals:
    void sizeChanged();

protected:
    QVector<Nodes> allowedNodes;
    Nodes currentNode;
    BoundRect *bound;
    bool enable;
    bool affectSize;
    BasicNode *node;
    qreal chanelSize;

protected slots:
    void onNodeChangeScale(qreal factor, QSizeF size);
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
