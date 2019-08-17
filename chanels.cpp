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
    , node(new BasicNode(bound))
{
//    hide();
}

QRectF BasicChanel::boundingRect() const
{
    return bound->getBound();
}

void BasicChanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    node->paint(painter, option, widget);
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
            this, &BasicChanel::onNodeChange);

    nodeLO->addWidget(nodeL);
    nodeLO->addWidget(nodeCB);
    mL->addLayout(nodeLO);

    mL->addLayout(node->getSettingsLayout());

    return mL;
}

void BasicChanel::changeScale(qreal scale)
{
    bound->setScale(scale);
}

void BasicChanel::setBoundSize(QSizeF size)
{
    bound->setSize(size);
}

void BasicChanel::onNodeChange(int index)
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
    update();
}

DiffuseChanel::DiffuseChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel (br, parent)
{
    node = new ImageNode(bound);
    currentNode = Nodes::ImageN;

    allowedNodes.push_back(Nodes::ImageN);
    allowedNodes.push_back(Nodes::TextN);
//    allowedNodes.push_back(Nodes::ShapeN);
}

//void DiffuseChanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    node->paint(painter, option, widget);
//}

//QLayout *DiffuseChanel::getSettingsLayout()
//{
//    QVBoxLayout *mL = new QVBoxLayout();
//    // Enable check box
//    QHBoxLayout *enableLO = new QHBoxLayout();
//    QLabel *enableL = new QLabel(tr("Enable"));
//    QCheckBox *enableCB = new QCheckBox();

//    enableCB->setCheckState(Qt::Checked);

//    enableLO->addWidget(enableL);
//    enableLO->addWidget(enableCB);
//    mL->addLayout(enableLO);
//    // Node selector
//    QHBoxLayout *nodeLO = new QHBoxLayout();
//    QLabel *nodeL = new QLabel(tr("Node type"));
//    QComboBox *nodeCB = new QComboBox();

//    nodeCB->addItem(tr("Image"));
//    nodeCB->addItem(tr("Text"));
//    nodeCB->addItem(tr("Shape"));

//    nodeLO->addWidget(nodeL);
//    nodeLO->addWidget(nodeCB);
//    mL->addLayout(nodeLO);

//    mL->addLayout(node->getSettingsLayout());

//    return mL;
//}

SpecularChanel::SpecularChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel (br, parent)
{
    node = new ImageNode(bound);
    currentNode = Nodes::ImageN;

    allowedNodes.push_back(Nodes::ImageN);
    allowedNodes.push_back(Nodes::TextN);
}
