#include "chanels.h"

#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QVector2D>

BasicChanel::BasicChanel(QGraphicsItem *parent)
    : QGraphicsObject (parent)
    , bound(0, 0, 50, 50)
    , enable(true)
    , node(new BasicNode())
{

}

QRectF BasicChanel::boundingRect() const
{
    return bound;
}

void BasicChanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{


}

void BasicChanel::changeScale(qreal scale)
{
    setBoundSize(boundingRect().size() / this->scale() * scale);
    setScale(scale);
}

void BasicChanel::setBoundSize(QSizeF size)
{
    prepareGeometryChange();
    bound.setSize(size);
    emit sizeChanged();
}

DiffuseChanel::DiffuseChanel(QGraphicsItem *parent)
    : BasicChanel (parent)
{
    node = new ImageNode();

    connect(node, &BasicNode::scaleChanged,
            this, &BasicChanel::changeScale);

    connect(node, &BasicNode::boundSizeChanged,
            this, &BasicChanel::setBoundSize);
}

void DiffuseChanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QRectF r = boundingRect();
//    r.adjust(5, 5, -5, -5);
//    painter->drawEllipse(r);
    node->paint(painter, option, widget);
}

QLayout *DiffuseChanel::getSettingsLayout()
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

    nodeCB->addItem(tr("Image"));
    nodeCB->addItem(tr("Text"));
    nodeCB->addItem(tr("Shape"));

    nodeLO->addWidget(nodeL);
    nodeLO->addWidget(nodeCB);
    mL->addLayout(nodeLO);

    mL->addLayout(node->getSettingsLayout());

    return mL;
}
