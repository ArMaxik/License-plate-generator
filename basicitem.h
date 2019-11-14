#ifndef BASICITEM_H
#define BASICITEM_H

#include "chanels.h"

#include <memory>

#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QFont>
#include <QVector2D>
#include <QXmlStreamWriter>

#include <QDebug>

class BasicItem : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicItem();
    ~BasicItem() override {
        delete bound;
        delete diffuseCh;
        delete specularCh;
        delete normalCh;
    }
    friend BoundRect;

    virtual void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;
    QRectF boundingRect() const override;

    virtual QLayout *getSettingsLayout();
    QString getName() { return name; }

    BasicChanel *getDiffuseChanel() const  { return diffuseCh; }
    BasicChanel *getSpecularChanel() const { return specularCh; }
    BasicChanel *getNormalChanel() const   { return normalCh; }

    BoundRect *getBoundRect() const        { return bound; }

    void setDiffuseChanel(BasicChanel *dChanel)  { diffuseCh = dChanel; }
    void setSpecularChanel(BasicChanel *sChanel) { specularCh = sChanel; }
    void setNormalChanel(BasicChanel *nChanel)   { normalCh = nChanel; }

    virtual void setUpChanels();
    void setName(const QString &newName) { name = newName; }

    enum Chanels { diffuseC, specularC, normalC };

    virtual void toXml(QXmlStreamWriter &stream);

signals:
    void changed();
    void layoutChanged();
    void needDelete();

protected:
    QString name;
    BoundRect *bound;
    Chanels sizeAffectedCh;
    Chanels shownC;

    BasicChanel *diffuseCh;
    BasicChanel *specularCh;
    BasicChanel *normalCh;

    QLayout *setUpBasicLayout();

//    bool diffuseChToSpec;

protected slots:
    void onChanelChanged() { emit changed(); }
    void onShownChanelChange(int index);
    void changeSizeAffectedCh(int newCh);
    void updateAllChanels();
    void answerChanelAskForNode(BasicChanel *chanel, NormalChanel::DefineBy node);
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

    void toXml(QXmlStreamWriter &stream) override;

protected:
    QLayout *setUpBasicLayout();

};

#endif // BASICITEM_H
