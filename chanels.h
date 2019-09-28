#ifndef CHANELS_H
#define CHANELS_H

#include "nodes.h"
#include "abstarctgraphicseffect.h"

#include <QtDebug>

#include <QGraphicsObject>
#include <QLayout>
#include <QStack>
#include <QFrame>

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
    void setAffectSize(bool affect);
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

    QImage chanelBuffer;
    BasicNode *node;
    AbstarctGraphicsEffect *effect;
    bool needRedraw;

    QColor defaultColor;

    virtual QFrame *formedSettingsFrame();

protected slots:
    void onNodeChangeScale(qreal factor, QSizeF size);
    void redrawChanel();
//    void onNodeChanged() { qDebug()<<"Hoba"; emit changed(); }
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

class NormalChanel : public BasicChanel
{
    Q_OBJECT
public:
    NormalChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

private:
    enum DefineBy { NormalMap, HeightMap };  // Order is restricted!
    DefineBy chanelDefBy;

    QFrame *formedSettingsFrame() override;
};

#endif // CHANELS_H
