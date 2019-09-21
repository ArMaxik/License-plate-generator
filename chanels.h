#ifndef CHANELS_H
#define CHANELS_H

#include "nodes.h"
#include <QtDebug>

#include <QGraphicsObject>
#include <QLayout>
#include <QStack>

#include <memory>

class BasicChanel : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QLayout *getSettingsLayout();
    void randomize();

    enum Nodes { ImageN, TextN, ShapeN, FillBackN, ImageBackN };

public slots:
    void changeScale(qreal scale)   { bound->setScale(scale); }
    void setBoundSize(const QSizeF &size)  { bound->setSize(size); }
    void setAffectSize(bool affect) { affectSize = affect; }
    void addAllowedNode(Nodes node) { allowedNodes.push_back(node); }
    void setNode(int index);
    void setNode(Nodes nodeType);
    void setDefaultColor(const QColor &color) { defaultColor = color; }

signals:
    void changed();
    void layoutChanged();

protected:
    QVector<Nodes> allowedNodes;
    Nodes currentNode;
    BoundRect *bound;
    bool enable;
    bool affectSize;
    qreal chanelSize;

    BasicNode *node;

    QColor defaultColor;

    QLayout *formedSettingsLayout();

protected slots:
    void onNodeChangeScale(qreal factor, QSizeF size);
//    void onNodeChanged() { qDebug()<<"Hoba"; emit changed(); }
};

class DiffuseChanel : public BasicChanel
{
    Q_OBJECT
public:
    DiffuseChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    QLayout *getSettingsLayout() override { return BasicChanel::getSettingsLayout(); }
};

class SpecularChanel : public BasicChanel
{
    Q_OBJECT
public:
    SpecularChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    QLayout *getSettingsLayout() override { return BasicChanel::getSettingsLayout(); }
};

class NormalChanel : public BasicChanel
{
    Q_OBJECT
public:
    NormalChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    QLayout *getSettingsLayout() override;

private:
    enum DefineBy { NormalMap, HeightMap };  // Order is restricted!
    DefineBy chanelDefBy;
};

#endif // CHANELS_H
