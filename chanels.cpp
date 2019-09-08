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
    , chanelSize(1.0)
    , node(nullptr)
    , defaultColor(Qt::white)
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

void BasicChanel::formedSettingsLayout()
{
    settingsLayout.layout()->clear();
    // Enable check box
    QHBoxLayout *enableLO = new QHBoxLayout();
    QLabel *enableL = new QLabel(tr("Enable"));
    QCheckBox *enableCB = new QCheckBox();

    enableCB->setCheckState(Qt::Checked);

    enableLO->addWidget(enableL);
    enableLO->addWidget(enableCB);
    settingsLayout.layout()->addLayout(enableLO);
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
        case Nodes::ImageBackN:
            nodeCB->addItem(tr("Tilled Image"));
            break;
        case Nodes::FillBackN:
            nodeCB->addItem(tr("Fill color"));
            break;
        }
    }
    nodeCB->setCurrentIndex(currentNode);

    connect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BasicChanel::setNode);

    nodeLO->addWidget(nodeL);
    nodeLO->addWidget(nodeCB);
    settingsLayout.layout()->addLayout(nodeLO);

    settingsLayout.layout()->addLayout(node->getSettingsLayout());
}

QLayout *BasicChanel::getSettingsLayout()
{
    formedSettingsLayout();
    return settingsLayout.layout();
}

void BasicChanel::randomize()
{
//    node->randomize()
}

void BasicChanel::setNode(int index)
{
    if(node != nullptr) {
        disconnect(node, &BasicNode::changed,
                   this, &BasicChanel::onNodeChanged);
        delete node;
        node = nullptr;
    }

    switch (allowedNodes[index]) {
    case Nodes::TextN:
         currentNode = Nodes::TextN;
         node = new TextNode(bound);
        break;
    case Nodes::ImageN:
        currentNode = Nodes::ImageN;
         node = new ImageNode(bound);
        break;
    case Nodes::ImageBackN:
        currentNode = Nodes::ImageN;
//        node = new ImageNode(bound);
        break;
    case Nodes::FillBackN:
        currentNode = Nodes::ImageN;
        node = new FillBasckgroundNode(bound, defaultColor);
        break;
    }
    node->setAffectSize(affectSize);
    connect(node, &BasicNode::changed,
               this, &BasicChanel::onNodeChanged);
    update();
    formedSettingsLayout();
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

//    allowedNodes.push_back(Nodes::ImageN);
//    allowedNodes.push_back(Nodes::TextN);
//    allowedNodes.push_back(Nodes::ShapeN);

//    setNode(0);
}


SpecularChanel::SpecularChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel (br, parent)
{
//    allowedNodes.push_back(Nodes::ImageN);
//    allowedNodes.push_back(Nodes::TextN);

//    setNode(0);
}

NormalChanel::NormalChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel(br, parent)
{

}
