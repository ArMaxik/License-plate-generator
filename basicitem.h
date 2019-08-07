#ifndef BASICITEM_H
#define BASICITEM_H

#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QFont>

class BasicItem : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QWidget *getSettingsWidget();
    QString getName();

protected:
    QRectF bound;
    QString name;

};

class StaticImageItem : public BasicItem
{
    Q_OBJECT
public:
    StaticImageItem(QGraphicsItem *parent = nullptr);

    QWidget *getSettingsWidget() override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QImage *img;
};

class StaticTextItem : public BasicItem
{
    Q_OBJECT
public:
    StaticTextItem(QGraphicsItem *parent = nullptr);

    QWidget *getSettingsWidget() override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
private:
    QString text;
    QFont font;
};

#endif // BASICITEM_H
