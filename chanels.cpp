#include "chanels.h"

#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QVector2D>

#include <QtDebug>


BasicChanel::BasicChanel(BoundRect *br, QGraphicsItem *parent)
    : QGraphicsObject (parent)
    , bound(br)
    , enable(true)
    , affectSize(false)
    , node(nullptr)
    , chanelSize(1.0)
{
}

QRectF BasicChanel::boundingRect() const
{
    return bound->getBound();
}

void BasicChanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->scale(node->getScale(), node->getScale());
    node->paint(painter, option, widget);

    painter->restore();
}

QLayout *BasicChanel::getSettingsLayout()
{
    QVBoxLayout *mL = new QVBoxLayout();
    // Enable check box
    QHBoxLayout *enableLO = new QHBoxLayout();
    QLabel *enableL = new QLabel(tr("Enable"));
    QCheckBox *enableCB = new QCheckBox();

    enableCB->setCheckState(Qt::Checked);

    enableLO->addWidget(enableL);
    enableLO->addWidget(enableCB);
    mL->addLayout(enableLO);
    // Node selector
    QHBoxLayout *nodeLO = new QHBoxLayout();
    QLabel *nodeL = new QLabel(tr("Node type"));
    QComboBox *nodeCB = new QComboBox();

    foreach(Nodes node, allowedNodes) {
        switch (node) {
        case Nodes::TextN:
            nodeCB->addItem(tr("Text"));
            break;
        case Nodes::ImageN:
            nodeCB->addItem(tr("Image"));
            break;
        case Nodes::ShapeN:
            nodeCB->addItem(tr("Shape"));
            break;
        }
    }
    nodeCB->setCurrentIndex(currentNode);

    connect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BasicChanel::setNode);

    nodeLO->addWidget(nodeL);
    nodeLO->addWidget(nodeCB);
    mL->addLayout(nodeLO);

    mL->addLayout(node->getSettingsLayout());

    return mL;
}

void BasicChanel::setNode(int index)
{
    delete node;

    switch (index) {
    case Nodes::TextN:
         currentNode = Nodes::TextN;
         node = new TextNode(bound);
        break;
    case Nodes::ImageN:
        currentNode = Nodes::ImageN;
         node = new ImageNode(bound);
        break;
    }
    node->setAffectSize(affectSize);

    update();
}

void BasicChanel::onNodeChangeScale(qreal factor, QSizeF size)
{
    chanelSize = factor;
    if(affectSize) {
        bound->setSize(size * factor);
    }
}

DiffuseChanel::DiffuseChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel (br, parent)
{
    setNode(Nodes::ImageN);

    allowedNodes.push_back(Nodes::ImageN);
    allowedNodes.push_back(Nodes::TextN);

//    allowedNodes.push_back(Nodes::ShapeN);
}


SpecularChanel::SpecularChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel (br, parent)
{
    node = new ImageNode(bound);
    currentNode = Nodes::ImageN;

    allowedNodes.push_back(Nodes::ImageN);
    allowedNodes.push_back(Nodes::TextN);
}
