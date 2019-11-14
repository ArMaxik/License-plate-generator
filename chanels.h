#ifndef CHANELS_H
#define CHANELS_H

#include "nodes.h"
#include "abstarctgraphicseffect.h"

#include <QtDebug>

#include <QGraphicsObject>
#include <QLayout>
#include <QSet>
#include <QFrame>
#include <QXmlStreamWriter>


#include <memory>

class BasicChanel : public QGraphicsObject
{
    Q_OBJECT
public:
    BasicChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QLayout *getSettingsLayout();
    NodeHolder* getNode() { return nodeHolder; }
    void randomize();
    virtual void toXml(QXmlStreamWriter &stream) = 0;


    enum Nodes { ImageN, TextN, ShapeN, FillBackN, ImageBackN, DiffuseChanelLinkN };


public slots:
    void update() { needRedraw = true; }
    void changeScale(qreal scale)   { bound->setScale(scale); }
    void setBoundSize(const QSizeF &size)  { bound->setSize(size); }
    void setAffectSize(bool affect);
    void addAllowedNode(Nodes node) { allowedNodes.push_back(node); }

    void setNode(int index);
    void setNode(Nodes nodeType);
    void setNode(BasicNode *initNode);
    void setNode(NodeHolder *initNode);
    void setNewNode(BasicNode *initNode);

    void setCurrentNode(Nodes currNode) { currentNode = currNode; }

    void setDefaultColor(const QColor &color) { defaultColor = color; }
    void redrawChanel();


signals:
    void changed();
    void layoutChanged();
    void askForDiffuseNode(BasicChanel *chanel);

protected:
    QVector<Nodes> allowedNodes;
    Nodes currentNode;
    BoundRect *bound;
    bool enable;
    bool affectSize;
    qreal chanelSize;

    QImage chanelBuffer;
    NodeHolder *nodeHolder;
    AbstarctGraphicsEffect *effect;
    bool needRedraw;

    QColor defaultColor;  // Какой-то костыль :(

    virtual QFrame *formedSettingsFrame();

protected slots:
    void onNodeChangeScale(qreal factor, QSizeF size);
//    void onNodeChanged() { qDebug()<<"Hoba"; emit changed(); }
};

class DiffuseChanel : public BasicChanel
{
    Q_OBJECT
    void toXml(QXmlStreamWriter &stream) override;

public:
    DiffuseChanel(BoundRect *br, QGraphicsItem *parent = nullptr);
};

class SpecularChanel : public BasicChanel
{
    Q_OBJECT
    void toXml(QXmlStreamWriter &stream) override;

public:
    SpecularChanel(BoundRect *br, QGraphicsItem *parent = nullptr);
};

class NormalChanel : public BasicChanel
{
    Q_OBJECT
public:
    NormalChanel(BoundRect *br, QGraphicsItem *parent = nullptr);

    enum DefineBy { NormalMap, HeightMap, NONE };  // Order is restricted!
    void toXml(QXmlStreamWriter &stream) override;


signals:
    void askForNode(BasicChanel *chanel, DefineBy defNode);

private:
    QFrame *formedSettingsFrame() override;
    DefineBy chanelDefBy;



private slots:
    void onDefinaBychange(int index);
};

#endif // CHANELS_H
