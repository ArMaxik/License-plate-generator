#ifndef BASICITEM_H
#define BASICITEM_H

#include "chanels.h"

#include <memory>

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
    QString getName() { return name; }

    BasicChanel *getDiffuseChanel() { return diffuseCh; }
    BasicChanel *getSpecularChanel() { return specularCh; }
    BasicChanel *getNormalChanel() { return normalCh; }

    BoundRect *getBoundRect() const { return bound; }

    virtual void setUpChanels();

    enum Chanels { diffuseC, specularC, normalC };

signals:
    void changed();
    void layoutChanged();

protected:
    QString name;
    BoundRect *bound;
    Chanels sizeAffectedCh;

    BasicChanel *diffuseCh;
    BasicChanel *specularCh;
    BasicChanel *normalCh;

    QLayout *setUpBasicLayout();

    Chanels shownC;
    bool diffuseChToSpec;

protected slots:
    void onChanelChanged() { emit changed(); }
    void onShownChanelChange(int index);
    void changeSizeAffectedCh(int newCh);
    void updateAllChanels();
    void answerChanelAskForNode(BasicChanel *chanel, BasicChanel::DefineBy node);
    void answerChanelAskForDiffuseNode(BasicChanel *chanel);
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
