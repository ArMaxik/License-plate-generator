#include "chanels.h"
#include "heigthtonormalgraphicseffect.h"

#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QVector2D>

#include <QtDebug>

#include <QGraphicsBlurEffect>

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

QLayout *BasicChanel::getSettingsLayout()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addStretch();
    // Enable check box
    QHBoxLayout *enableLO = new QHBoxLayout();
    QLabel *enableL = new QLabel(tr("Enable"));
    QCheckBox *enableCB = new QCheckBox();

    enableCB->setCheckState(Qt::Checked);

    enableLO->addWidget(enableL);
    enableLO->addWidget(enableCB);
    layout->addLayout(enableLO);
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
            this, QOverload<int>::of(&BasicChanel::setNode));
    connect(nodeCB, &QObject::destroyed,
            this, [=](){ disconnect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                    this, QOverload<int>::of(&BasicChanel::setNode)); });

    nodeLO->addWidget(nodeL);
    nodeLO->addWidget(nodeCB);
    layout->addLayout(nodeLO);

    layout->addLayout(node->getSettingsLayout());
    layout->addStretch();

    return layout;
}

void BasicChanel::randomize()
{
//    node->randomize()
}

void BasicChanel::setNode(int index)
{
    setNode(allowedNodes[index]);
}

void BasicChanel::setNode(Nodes nodeType)
{
    if(node != nullptr) {
        disconnect(node, &BasicNode::changed,
                   this, &BasicChanel::changed);
        delete node;
        node = nullptr;
    }

    switch (nodeType) {
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
               this, &BasicChanel::changed);
    update();
    emit layoutChanged();
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
    , chanelDefBy(NormalMap)
{
    //    setGraphicsEffect(new HeigthToNormalGraphicsEffect());
}

QLayout *NormalChanel::getSettingsLayout()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addStretch();
    // Enable check box
    QHBoxLayout *enableLO = new QHBoxLayout();
    QLabel *enableL = new QLabel(tr("Enable"));
    QCheckBox *enableCB = new QCheckBox();

    enableCB->setCheckState(Qt::Checked);

    enableLO->addWidget(enableL);
    enableLO->addWidget(enableCB);
    layout->addLayout(enableLO);
    // Normal type
    QHBoxLayout *typeLO = new QHBoxLayout();
    QLabel *typeL = new QLabel(tr("Define normal chanel by"));
    QComboBox *typeCB = new QComboBox();

    typeCB->addItem(tr("Normal map")); // Order is restricted!
    typeCB->addItem(tr("Height map")); // Order is restricted!
    typeCB->setCurrentIndex(chanelDefBy);

    connect(typeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                chanelDefBy = DefineBy(index);
                if(chanelDefBy == NormalMap) setNode(Nodes::ImageN);
                emit layoutChanged();
            });

    typeLO->addWidget(typeL);
    typeLO->addWidget(typeCB);
    layout->addLayout(typeLO);

    if(chanelDefBy == HeightMap){
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
                this, QOverload<int>::of(&BasicChanel::setNode));
        connect(nodeCB, &QObject::destroyed,
                this, [=](){ disconnect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                        this, QOverload<int>::of(&BasicChanel::setNode)); });

        nodeLO->addWidget(nodeL);
        nodeLO->addWidget(nodeCB);
        layout->addLayout(nodeLO);
    }

    layout->addLayout(node->getSettingsLayout());
    layout->addStretch();

    return layout;
}
