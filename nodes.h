#ifndef NODES_H
#define NODES_H

#include "boundrect.h"
#include "properties.h"

#include <QLayout>
#include <QImage>
#include <QGraphicsItem>

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

signals:
    void scaleChanged(qreal factor);
    void boundSizeChanged(QSizeF size);

protected:
    QList<BasicPropertie*> properites;
    BoundRect *bound;

};

class ImageNode : public BasicNode
{
    Q_OBJECT
public:
    ImageNode(BoundRect *br);

    void paint(QPainter *painter,
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

#endif // NODES_H
