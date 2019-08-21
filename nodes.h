#ifndef NODES_H
#define NODES_H

#include "boundrect.h"
#include "properties.h"

#include <QLayout>
#include <QImage>
#include <QGraphicsItem>
#include <QPen>

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
    void setAffectSize(bool affect) { affectSize = affect; }
    qreal getScale() const { return scale; }

signals:
//    void scaleChanged(qreal factor, QSizeF size);
    void scaleChanged(qreal newScale);
    void boundSizeChanged(QSizeF size);

protected:
    QList<BasicPropertie*> properites;
    BoundRect *bound;
    bool affectSize;
    qreal scale;
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
    void changeSizeW(int w);
    void changeSizeH(int h);
};

class TextNode : public BasicNode
{
    Q_OBJECT
public:
    TextNode(BoundRect *br);
//    ~TextNode() override;

    virtual void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

protected:
    StringPropertie *string;
    NumberPropertie *fontSize;
    ColorPropertie *color;
    QFont font;
    QPen pen;
    bool updateBound;

protected slots:
    void stringChanged(QString newStr);
};

class FillBasckgroundNode : public BasicNode
{
    Q_OBJECT
public:
    FillBasckgroundNode(BoundRect *br);

    virtual void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
protected:
    ColorPropertie *color;
};

#endif // NODES_H
