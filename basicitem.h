#ifndef BASICITEM_H
#define BASICITEM_H

#include "chanels.h"

#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QFont>
#include <QVector2D>
#include <QDebug>

class BasicItem : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicItem();
    friend BoundRect;

    virtual void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
    QRectF boundingRect() const override;

    virtual QLayout *getSettingsLayout();
    QString getName();

    BasicChanel *getDiffuseChanel();
    BasicChanel *getSpecularChanel();

    BoundRect *getBoundRect() const { return bound; }

    virtual void setUpChanels();

    enum Chanels { diffuseC, specularC, normalC };

signals:
//    void sizeChanged();
    void changed();

protected:
    QString name;
    BoundRect *bound;

    BasicChanel *diffuseCh;
    BasicChanel *specularCh;

    QLayout *setUpBasicLayout();

    Chanels shownC;

protected slots:
    void onChanelChanged() { emit changed(); }
    void onShownChanelChange(int index);

};

class Canvas : public BasicItem
{
    Q_OBJECT
public:
    Canvas(QSize size);

    void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    QLayout *getSettingsLayout() override;

    void setUpChanels() override;

protected:
    QLayout *setUpBasicLayout();

};

#endif // BASICITEM_H
