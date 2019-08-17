#ifndef BASICITEM_H
#define BASICITEM_H

#include "chanels.h"

#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QFont>
#include <QVector2D>

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

    enum Chanels { diffuseC, specularC, normalC };

signals:
    void sizeChanged();

protected:
    QString name;
    BoundRect *bound;

    BasicChanel *diffuseCh;
    BasicChanel *specularCh;

    QLayout *setUpBasicLayout();

    Chanels shownC;

protected slots:
    void onChanelSizeChange();
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

};

#endif // BASICITEM_H
