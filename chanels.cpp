#include "chanels.h"
//#include "heigthtonormalgraphicseffect.h"

#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QVector2D>

#include <QtDebug>

#include <QGraphicsBlurEffect>

// НУЖНО ОТРЕВЬЮРИТЬ

BasicChanel::BasicChanel(BoundRect *br, QGraphicsItem *parent)
    : QGraphicsObject (parent)
    , bound(br)
    , enable(true)
    , affectSize(false)
    , chanelSize(1.0)
    , chanelBuffer(QImage())
    , nodeHolder(new NodeHolder())
    , effect(new BasicGraphicsEffect())
    , needRedraw(true)
    , defaultColor(Qt::white)
{
    connect(this, &BasicChanel::changed,
            this, &BasicChanel::redrawChanel);
}

QRectF BasicChanel::boundingRect() const
{
    return bound->getBound();
}

void BasicChanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!enable) {
        return;
    }
    if(needRedraw){
        QImage base = QImage(QSize(boundingRect().size().width(), boundingRect().size().height()), QImage::Format_RGBA8888);
        if(base.isNull()) {
            chanelBuffer.fill(0);
            painter->drawImage(0, 0, chanelBuffer);
            return;
        }

        base.fill(0);
        QPainter painterBase(&base);

        painterBase.setRenderHint(QPainter::SmoothPixmapTransform);
        painterBase.scale(nodeHolder->getNode()->getScale(), nodeHolder->getNode()->getScale());
        nodeHolder->getNode()->paint(&painterBase, option, widget);
        chanelBuffer = effect->apply(&base);
        needRedraw = false;
    }
    painter->drawImage(0, 0, chanelBuffer);
}

QLayout *BasicChanel::getSettingsLayout()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addStretch();
    // Enable check box
    QHBoxLayout *enableLO = new QHBoxLayout();
    QLabel *enableL = new QLabel(tr("Enable"));
    QCheckBox *enableCB = new QCheckBox();

    enableCB->setCheckState(enable ? Qt::Checked : Qt::Unchecked);

    connect(enableCB, &QCheckBox::stateChanged,
            this, [this](int i) { enable = i == Qt::Checked; emit changed(); emit layoutChanged(); } );

    enableLO->addWidget(enableL);
    enableLO->addWidget(enableCB);
    layout->addLayout(enableLO);

    layout->addWidget(formedSettingsFrame());

    layout->addStretch();

    return layout;
}

void BasicChanel::randomize()
{
    //    node->randomize()
}

void BasicChanel::setAffectSize(bool affect)
{
     affectSize = affect;
     if(nodeHolder != nullptr) {
         nodeHolder->getNode()->setAffectSize(affect);
     }
}

void BasicChanel::setNode(int index)
{
    setNode(allowedNodes[index]);
}

void BasicChanel::setNode(Nodes nodeType)
{
    if(nodeHolder != nullptr) {
        disconnect(nodeHolder->getNode(), &BasicNode::changed,
                   this, &BasicChanel::changed);
//        nodeHolder->clear();  // set nullptr
    }
    if(currentNode == Nodes::DiffuseChanelLinkN) {
        nodeHolder = new NodeHolder();
    }
    switch (nodeType) {
    case Nodes::TextN:
         currentNode = Nodes::TextN;
         nodeHolder->setNode(new TextNode(bound));
        break;
    case Nodes::ImageN:
        currentNode = Nodes::ImageN;
         nodeHolder->setNode(new ImageNode(bound));
        break;
    case Nodes::ImageBackN:
        currentNode = Nodes::ImageN;
//        node = new ImageNode(bound);
        break;
    case Nodes::FillBackN:
        currentNode = Nodes::FillBackN;
        nodeHolder->setNode(new FillBasckgroundNode(bound, defaultColor));
        break;
    case Nodes::DiffuseChanelLinkN:
        currentNode = Nodes::DiffuseChanelLinkN;
        emit askForDiffuseNode(this);
        return; // BAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;
    }
    nodeHolder->getNode()->setAffectSize(affectSize);
    connect(nodeHolder->getNode(), &BasicNode::changed,
               this, &BasicChanel::changed);
    needRedraw = true;
    update();
    emit layoutChanged();
    emit changed();
}

void BasicChanel::setNode(BasicNode *initNode)
{
//    nodeHolder.clear();
    nodeHolder->setNode(initNode);
    needRedraw = true;
    update();
    emit changed();
    emit layoutChanged();
}

void BasicChanel::setNode(NodeHolder *initNode)
{
    nodeHolder = initNode;
    emit layoutChanged();
}

QFrame *BasicChanel::formedSettingsFrame()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    // Node selector
    QHBoxLayout *nodeLO = new QHBoxLayout();
    QLabel *nodeL = new QLabel(tr("Node type"));
    QComboBox *nodeCB = new QComboBox();

    foreach(Nodes node, allowedNodes) {
        qDebug() << node;
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
        case Nodes::DiffuseChanelLinkN:
            nodeCB->addItem(tr("Diffuse chanel"));
        }
    }
    nodeCB->setCurrentIndex(allowedNodes.indexOf(currentNode));
    qDebug() << "C" << currentNode;
    qDebug() << "I" << allowedNodes.indexOf(currentNode);

    connect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, QOverload<int>::of(&BasicChanel::setNode));
    connect(nodeCB, &QObject::destroyed,
            this, [=](){ disconnect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                    this, QOverload<int>::of(&BasicChanel::setNode)); });

    nodeLO->addWidget(nodeL);
    nodeLO->addWidget(nodeCB);
    layout->addLayout(nodeLO);

    if(currentNode != Nodes::DiffuseChanelLinkN){
        layout->addLayout(nodeHolder->getNode()->getSettingsLayout());
        layout->addStretch();
    }

    QFrame *frame = new QFrame();
    frame->setLayout(layout);
    frame->setEnabled(enable);

    return frame;
}

void BasicChanel::onNodeChangeScale(qreal factor, QSizeF size)
{
    chanelSize = factor;
    if(affectSize) {
        bound->setSize(size * factor);
    }
}

void BasicChanel::redrawChanel()
{
    needRedraw = true;
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
//    addAllowedNode(Nodes::DiffuseChanelLinkN);
}

NormalChanel::NormalChanel(BoundRect *br, QGraphicsItem *parent)
    : BasicChanel(br, parent)
    , chanelDefBy(DefineBy::NormalMap)
{
//    addAllowedNode(Nodes::DiffuseChanelLinkN);
    chanelDefBy = NormalChanel::DefineBy::NormalMap;
}

QFrame *NormalChanel::formedSettingsFrame()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    // Normal type
    QHBoxLayout *typeLO = new QHBoxLayout();
    QLabel *typeL = new QLabel(tr("Define normal chanel by"));
    QComboBox *typeCB = new QComboBox();

    typeCB->addItem(tr("Normal map"));      // Order is restricted!
    typeCB->addItem(tr("Height map"));      // Order is restricted!
//    typeCB->addItem(tr("Specular Chanel")); // Order is restricted!
    typeCB->setCurrentIndex(chanelDefBy);

    connect(typeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NormalChanel::onDefinaBychange);

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
            case Nodes::DiffuseChanelLinkN:
                nodeCB->addItem(tr("Diffuse chanel"));
                break;
            }
        }
        nodeCB->setCurrentIndex(allowedNodes.indexOf(currentNode));

        connect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, QOverload<int>::of(&BasicChanel::setNode));
        connect(nodeCB, &QObject::destroyed,
                this, [=](){ disconnect(nodeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                        this, QOverload<int>::of(&BasicChanel::setNode)); });

        nodeLO->addWidget(nodeL);
        nodeLO->addWidget(nodeCB);
        layout->addLayout(nodeLO);
    }

    if(currentNode != Nodes::DiffuseChanelLinkN){
        layout->addLayout(nodeHolder->getNode()->getSettingsLayout());
        layout->addStretch();
    }

    QFrame *frame = new QFrame();
    frame->setLayout(layout);
    frame->setEnabled(enable);

    return frame;
}

void NormalChanel::onDefinaBychange(int index)
{
    chanelDefBy = DefineBy(index);
    if(chanelDefBy == DefineBy::NormalMap) {
        delete effect;
        effect = new BasicGraphicsEffect();
    } else {
        delete effect;
        effect = new NormalHeightGraphicsEffect();
    }

//    if(chanelDefBy == DefineBy::SpecularChanel) {
//        emit askForNode(this, DefineBy::SpecularChanel);
//    } else {
    setNode(Nodes::ImageN);

//    }

    needRedraw = true;
    emit layoutChanged();
    emit changed();
}
