#ifndef BASICITEM_H
#define BASICITEM_H

#include <QGraphicsItem>
#include <QVBoxLayout>

class BasicItem : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QLayout *getSettingsLayout();

private:
    QRectF bound;

};

class StaticImageItem : public BasicItem
{
    Q_OBJECT
public:
    StaticImageItem(QGraphicsItem *parent = nullptr);

    QLayout *getSettingsLayout() override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QImage *img;
};

class StaticTextImage : public BasicItem
{

};

#endif // BASICITEM_H
