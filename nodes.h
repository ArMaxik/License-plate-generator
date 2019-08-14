#ifndef NODES_H
#define NODES_H

#include "properties.h"

#include <QLayout>
#include <QImage>
#include <QGraphicsItem>

class BasicNode : public QObject
{
    Q_OBJECT
public:
    BasicNode();
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

};

class ImageNode : public BasicNode
{
    Q_OBJECT
public:
    ImageNode();

    void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;


protected:
    QImage image;

    NumberPropertie *width;
    NumberPropertie *height;

protected slots:
    void reloadImage(QString newPath);
    void changeSizeW(int w);
    void changeSizeH(int h);
};

#endif // NODES_H
