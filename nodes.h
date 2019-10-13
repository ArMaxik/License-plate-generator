#ifndef NODES_H
#define NODES_H

#include "boundrect.h"
#include "properties.h"

#include <QLayout>
#include <QImage>
#include <QGraphicsItem>
#include <QPen>
#include <QPointer>
#include <QDebug>

class BasicNode : public QObject
{
    Q_OBJECT
public:
    BasicNode(BoundRect *br);
    ~BasicNode();

    QLayout *getSettingsLayout();

    virtual void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    virtual void setAffectSize(bool affect) { affectSize = affect; }
    qreal getScale() const { return scale; }

public slots:
    void randomize();

signals:
//    void scaleChanged(qreal factor, QSizeF size);
//    void scaleChanged(qreal newScale);
//    void boundSizeChanged(QSizeF size);
    void changed();
    void layoutChanged();

protected:
    QList<BasicPropertie*> properites;
    BoundRect *bound;
    bool affectSize;
    qreal scale;

    void makeAllConnections();
};

class NodeHolder : public QObject
{
    Q_OBJECT
public:
    NodeHolder(BasicNode *initNode = nullptr)
        : node(initNode) {}

    void setNode(BasicNode *initNode) {
//        if(!node.isNull()) node.clear();
//        node = QPointer<BasicNode>(initNode);
        if(node != nullptr) delete node;
        node = initNode;
    }

    BasicNode *getNode() {
        return node;
    }

private:
//    QPointer<BasicNode> node;
    BasicNode *node;
};

class ImageNode : public BasicNode
{
    Q_OBJECT
public:
    ImageNode(BoundRect *br);
//    ~ImageNode() override;

    virtual void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;


protected:
    ImagePropertie *image;
    NumberPropertie *width;
    NumberPropertie *height;

protected slots:
    void reloadImage();
    void changeSizeW();
    void changeSizeH();
};

class TextNode : public BasicNode
{
    Q_OBJECT
public:
    TextNode(BoundRect *br);
//    ~TextNode() override;

    void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
    void setAffectSize(bool affect) override { affectSize = affect; updateBound(); }


protected:
    StringPropertie *string;
    NumberPropertie *fontSize;
    ColorPropertie *color;
    QFont font;
    QPen pen;

    void updateBound();

protected slots:
    void stringChanged(QString newStr);
};

class FillBasckgroundNode : public BasicNode
{
    Q_OBJECT
public:
    FillBasckgroundNode(BoundRect *br, QColor defaultColor = Qt::black);

    virtual void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
protected:
    ColorPropertie *color;
};

#endif // NODES_H
