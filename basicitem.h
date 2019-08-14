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

    void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
    QRectF boundingRect() const override;
    BoundRect *getBoundRect() const;

    virtual QLayout *getSettingsLayout();
    QString getName();

    BasicChanel *getDiffuseChanel();

signals:
    void sizeChanged();

protected:
    QString name;
    BoundRect *bound;

    BasicChanel *diffuseCh;

    QLayout *setUpBasicLayout();

protected slots:
    void onChanelSizeChange();

};

class ControllGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ControllGraphicsItem(BasicItem *bi, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
    QRectF boundingRect() const override;

    BasicItem *getItem();

private:
    BasicItem *item;
    QRectF bound;

private slots:
    void setSize(QRectF rect);
};

#endif // BASICITEM_H
