#include "basicitem.h"
#include "heigthtonormalgraphicseffect.h"

#include <QPainter>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QFrame>
#include <QComboBox>
#include <QListWidget>

#include <QDebug>

BasicItem::BasicItem()
    : QGraphicsObject()
    , name(QObject::tr("Unnamed Item"))
    , bound(new BoundRect(QRectF(0.0, 0.0, 0.0, 0.0), this))
    , sizeAffectedCh(Chanels::diffuseC)
    , diffuseCh(new DiffuseChanel(bound))
    , specularCh(new SpecularChanel(bound))
    , normalCh(new NormalChanel(bound))
    , shownC(Chanels::diffuseC)
    , diffuseChToSpec(false)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    connect(this, &QGraphicsObject::xChanged,
            this, [this](){
        diffuseCh->setX(x());
        specularCh->setX(x());
        normalCh->setX(x());
        emit changed();
    });

    connect(this, &QGraphicsObject::yChanged,
            this, [this](){
        diffuseCh->setY(y());
        specularCh->setY(y());
        normalCh->setY(y());
        emit changed();
    });

    // Chanel changed, not layout
    connect(diffuseCh, &BasicChanel::changed,
            this, &BasicItem::updateAllChanels);

    connect(specularCh, &BasicChanel::changed,
            this, &BasicItem::updateAllChanels);

    connect(normalCh, &BasicChanel::changed,
            this, &BasicItem::updateAllChanels);
    //
//    connect(this, &BasicItem::changed,
//            this, &BasicItem::updateAllChanels);
    // Layout changed
    connect(diffuseCh, &BasicChanel::layoutChanged,
            this, &BasicItem::layoutChanged);

    connect(specularCh, &BasicChanel::layoutChanged,
            this, &BasicItem::layoutChanged);

    connect(normalCh, &BasicChanel::layoutChanged,
            this, &BasicItem::layoutChanged);
    // If chanel want node of other chanel
//    connect(normalCh, &NormalChanel::askForNode,
//            this, &BasicItem::answerChanelAskForNode);
    // If chanel want node of other chanel
    connect(specularCh, &BasicChanel::askForDiffuseNode,
            this, &BasicItem::answerChanelAskForDiffuseNode);
    connect(normalCh, &BasicChanel::askForDiffuseNode,
            this, &BasicItem::answerChanelAskForDiffuseNode);
}

void BasicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QImage res;
    switch (shownC) {
    case Chanels::diffuseC:
        diffuseCh->paint(painter, option, widget);
        break;
    case Chanels::specularC:
        specularCh->paint(painter, option, widget);
        break;
    case Chanels::normalC:
        normalCh->paint(painter, option, widget);
        break;
    }

    if(isSelected()) {
           QPen pen;
           pen.setColor(Qt::red);
           pen.setStyle(Qt::DotLine);
           pen.setWidth(2);
           painter->setPen(pen);

           painter->drawRect(boundingRect());
    }
}

QRectF BasicItem::boundingRect() const
{
    return bound->getBound();
}

QLayout *BasicItem::getSettingsLayout()
{
    QVBoxLayout *layout = new QVBoxLayout();

    QGroupBox *basicGroup = new QGroupBox(tr("Basic properties"));
    QGroupBox *diffuseGroup = new QGroupBox(tr("Diffuse chanel"));
    QGroupBox *specularGroup = new QGroupBox(tr("Specular chanel"));
    QGroupBox *normalGroup = new QGroupBox(tr("Normal chanel"));

//    diffuseGroup->setObjectName("myObject");
//    diffuseGroup->setStyleSheet("border: 1px solid red");

    basicGroup->setLayout(setUpBasicLayout());
    diffuseGroup->setLayout(diffuseCh->getSettingsLayout());
    specularGroup->setLayout(specularCh->getSettingsLayout());
    normalGroup->setLayout(normalCh->getSettingsLayout());

    layout->addWidget(basicGroup);
    layout->addWidget(diffuseGroup);
    layout->addWidget(specularGroup);
    layout->addWidget(normalGroup);

    return layout;
}

void BasicItem::setUpChanels()
{

    diffuseCh->addAllowedNode(BasicChanel::ImageN);
    diffuseCh->addAllowedNode(BasicChanel::TextN);
    diffuseCh->setNode(BasicChanel::ImageN);
    diffuseCh->setAffectSize(true);

    specularCh->addAllowedNode(BasicChanel::ImageN);
    specularCh->addAllowedNode(BasicChanel::TextN);
    specularCh->addAllowedNode(BasicChanel::DiffuseChanelLinkN);
    specularCh->setNode(BasicChanel::ImageN);

    normalCh->addAllowedNode(BasicChanel::ImageN);
    normalCh->addAllowedNode(BasicChanel::TextN);
    normalCh->addAllowedNode(BasicChanel::DiffuseChanelLinkN);
    normalCh->setNode(BasicChanel::ImageN);
    //    normalCh->setGraphicsEffect(new HeigthToNormalGraphicsEffect());
}

void BasicItem::toXml(QXmlStreamWriter &stream)
{
    stream.writeStartElement("BasicItem");
    stream.writeAttribute("name", name);
    stream.writeTextElement("X", QString::number(x()));
    stream.writeTextElement("Y", QString::number(y()));
    stream.writeTextElement("Z", QString::number(zValue()));
    diffuseCh->toXml(stream);
    specularCh->toXml(stream);
    normalCh->toXml(stream);
    stream.writeEndElement();
}

QLayout *BasicItem::setUpBasicLayout()
{
    QVBoxLayout *vlb = new QVBoxLayout();

    QLineEdit *nameLE = new QLineEdit(this->name);
    QLabel *nameL = new QLabel(tr("Items name"));

    QSpinBox *xSpinBox = new QSpinBox();
    QSpinBox *ySpinBox = new QSpinBox();
    xSpinBox->setRange(-5000, 5000);
    ySpinBox->setRange(-5000, 5000);
    xSpinBox->setValue(x());
    ySpinBox->setValue(y());

    QSpinBox *zSpinBox = new QSpinBox();
    zSpinBox->setRange(0, 100);
    zSpinBox->setValue(zValue());

    QLabel *xposL = new QLabel(tr("X position"));
    QLabel *yposL = new QLabel(tr("Y position"));
    QLabel *zposL = new QLabel(tr("Z index"));

    QHBoxLayout *nameLt = new QHBoxLayout();
    QHBoxLayout *xPosLt = new QHBoxLayout();
    QHBoxLayout *yPosLt = new QHBoxLayout();
    QHBoxLayout *zPosLt = new QHBoxLayout();

    vlb->addLayout(nameLt);
    vlb->addLayout(xPosLt);
    vlb->addLayout(yPosLt);
    vlb->addLayout(zPosLt);

    nameLt->addWidget(nameL);
    nameLt->addWidget(nameLE);

    xPosLt->addWidget(xposL);
    xPosLt->addWidget(xSpinBox);

    yPosLt->addWidget(yposL);
    yPosLt->addWidget(ySpinBox);

    zPosLt->addWidget(zposL);
    zPosLt->addWidget(zSpinBox);

    // Name change
    connect(nameLE, &QLineEdit::textChanged,
            this, [this](QString text){ if(text.isEmpty()) text = tr("Unnamed Item"); name = text; });
    // Position spinBox affect on object
    connect(xSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int x){ setX(x); });
    connect(ySpinBox,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int y){ setY(y); });
    connect(zSpinBox,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int z){ setZValue(z); });
    // Object position affect in spinBox
    connect(this, &QGraphicsObject::xChanged,
            xSpinBox, [this, xSpinBox](){xSpinBox->setValue(x());});
    connect(this, &QGraphicsObject::yChanged,
            ySpinBox, [this, ySpinBox](){ySpinBox->setValue(y());});


    // Choose what chanel to show
    QHBoxLayout *showLO = new QHBoxLayout();

    QLabel *showL = new QLabel(tr("Show chanel in edit"));
    QComboBox *showCB = new QComboBox();

    showCB->addItem(tr("Diffuse"));
    showCB->addItem(tr("Specular"));
    showCB->addItem(tr("Normal"));
    showCB->setCurrentIndex(shownC);

    showLO->addWidget(showL);
    showLO->addWidget(showCB);
    vlb->addLayout(showLO);

    connect(showCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BasicItem::onShownChanelChange);

    // SizeAffect
    QHBoxLayout *affSizeLO = new QHBoxLayout();
    QLabel *affSizeL = new QLabel(tr("Size affected chanel"));
    QComboBox *affSizeCB = new QComboBox();

    affSizeCB->addItem(tr("Diffuse"));
    affSizeCB->addItem(tr("Specular"));
    affSizeCB->addItem(tr("Normal"));
    affSizeCB->setCurrentIndex(sizeAffectedCh);

    connect(affSizeCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BasicItem::changeSizeAffectedCh);

    affSizeLO->addWidget(affSizeL);
    affSizeLO->addWidget(affSizeCB);
    vlb->addLayout(affSizeLO);

    return vlb;
}

void BasicItem::changeSizeAffectedCh(int newCh)
{
    switch (sizeAffectedCh) {
    case Chanels::diffuseC:
        diffuseCh->setAffectSize(false);
        break;
    case Chanels::specularC:
        specularCh->setAffectSize(false);
        break;
    case Chanels::normalC:
        normalCh->setAffectSize(false);
        break;
    }
    switch (newCh) {
    case Chanels::diffuseC:
        diffuseCh->setAffectSize(true);
        sizeAffectedCh = Chanels::diffuseC;
        break;
    case Chanels::specularC:
        specularCh->setAffectSize(true);
        sizeAffectedCh = Chanels::specularC;
        break;
    case Chanels::normalC:
        normalCh->setAffectSize(true);
        sizeAffectedCh = Chanels::normalC;
        break;
    }
}

void BasicItem::updateAllChanels()
{
    diffuseCh->update();
    specularCh->update();
    normalCh->update();
    update();
    emit changed();
}

void BasicItem::answerChanelAskForNode(BasicChanel *chanel, NormalChanel::DefineBy node)
{
//    switch(node){
//    case NormalChanel::DefineBy::SpecularChanel:
//        chanel->setNode(specularCh->getNode());
//        break;
//    }
}

void BasicItem::answerChanelAskForDiffuseNode(BasicChanel *chanel)
{
    chanel->setNode(diffuseCh->getNode());
}

void BasicItem::onShownChanelChange(int index)
{
    switch (index) {
    case Chanels::diffuseC:
        shownC = Chanels::diffuseC;
        break;
    case Chanels::specularC:
        shownC = Chanels::specularC;
        break;
    case Chanels::normalC:
        shownC = Chanels::normalC;
        break;
    }
    update();
}

// ======================[ Canvas ]============================================

Canvas::Canvas(QSize size)
    : BasicItem ()
{
    setFlag(QGraphicsItem::ItemIsMovable, false);

    name = "Canvas";
    bound->setSize(size);
}

void Canvas::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::white);
    painter->setPen(Qt::gray);
    painter->drawRect(boundingRect());

    switch (shownC) {
    case Chanels::diffuseC:
        diffuseCh->paint(painter, option, widget);
        break;
    case Chanels::specularC:
        specularCh->paint(painter, option, widget);
        break;
    case Chanels::normalC:
        normalCh->paint(painter, option, widget);
        break;
    }
}

QLayout *Canvas::getSettingsLayout()
{
    QVBoxLayout *layout = new QVBoxLayout();

    QGroupBox *basicGroup = new QGroupBox(tr("Basic properties"));
    QGroupBox *diffuseGroup = new QGroupBox(tr("Diffuse chanel"));
    QGroupBox *specularGroup = new QGroupBox(tr("Specular chanel"));
    QGroupBox *normalGroup = new QGroupBox(tr("Normal chanel"));

    basicGroup->setLayout(setUpBasicLayout());
    diffuseGroup->setLayout(diffuseCh->getSettingsLayout());
    specularGroup->setLayout(specularCh->getSettingsLayout());
    normalGroup->setLayout(normalCh->getSettingsLayout());

    layout->addWidget(basicGroup);
    layout->addWidget(diffuseGroup);
    layout->addWidget(specularGroup);
    layout->addWidget(normalGroup);

    return layout;
}

void Canvas::setUpChanels()
{
    diffuseCh->addAllowedNode(BasicChanel::FillBackN);
    diffuseCh->addAllowedNode(BasicChanel::ImageBackN);
    diffuseCh->setDefaultColor(Qt::white);
    diffuseCh->setNode(BasicChanel::FillBackN);

    specularCh->addAllowedNode(BasicChanel::FillBackN);
    specularCh->addAllowedNode(BasicChanel::ImageBackN);
    specularCh->setDefaultColor(Qt::black);
    specularCh->setNode(BasicChanel::FillBackN);

    normalCh->addAllowedNode(BasicChanel::FillBackN);
    normalCh->addAllowedNode(BasicChanel::ImageBackN);
    normalCh->setDefaultColor(QColor(127, 127, 255));
    normalCh->setNode(BasicChanel::FillBackN);
}

void Canvas::toXml(QXmlStreamWriter &stream)
{
    stream.writeStartElement("Canvas");
    stream.writeAttribute("name", name);
    stream.writeTextElement("width", QString::number(boundingRect().width()));
    stream.writeTextElement("height", QString::number(boundingRect().height()));
    diffuseCh->toXml(stream);
    specularCh->toXml(stream);
    normalCh->toXml(stream);
    stream.writeEndElement();
}

QLayout *Canvas::setUpBasicLayout()
{
    QVBoxLayout *vlb = new QVBoxLayout();

    QLineEdit *nameLE = new QLineEdit(this->name);
    QLabel *nameL = new QLabel(tr("Items name"));

    QSpinBox *wSpinBox = new QSpinBox();
    QSpinBox *xSpinBox = new QSpinBox();
    wSpinBox->setRange(0, 5000);
    xSpinBox->setRange(0, 5000);
    wSpinBox->setValue(boundingRect().width());
    xSpinBox->setValue(boundingRect().height());

    QLabel *wL = new QLabel(tr("Width"));
    QLabel *hL = new QLabel(tr("Height"));

    QHBoxLayout *nameLt = new QHBoxLayout();
    QHBoxLayout *wLt = new QHBoxLayout();
    QHBoxLayout *hLt = new QHBoxLayout();

    vlb->addLayout(nameLt);
    vlb->addLayout(wLt);
    vlb->addLayout(hLt);

    nameLt->addWidget(nameL);
    nameLt->addWidget(nameLE);

    wLt->addWidget(wL);
    wLt->addWidget(wSpinBox);

    hLt->addWidget(hL);
    hLt->addWidget(xSpinBox);

    // Name change
    connect(nameLE, &QLineEdit::textChanged,
            this, [this](QString text){ if(text.isEmpty()) text = tr("Unnamed Item"); name = text; });
    // Size spinBox affect on object
    connect(wSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int w){ bound->setWidth(w); updateAllChanels(); });
    connect(xSpinBox,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int h){ bound->setHeight(h); updateAllChanels(); });

    // Choose what chanel to show
    QHBoxLayout *showLO = new QHBoxLayout();
    QLabel *showL = new QLabel(tr("Show chanel in edit"));
    QComboBox *showCB = new QComboBox();

    showCB->addItem(tr("Diffuse"));
    showCB->addItem(tr("Specular"));
    showCB->addItem(tr("Normal"));
    showCB->setCurrentIndex(shownC);

    showLO->addWidget(showL);
    showLO->addWidget(showCB);
    vlb->addLayout(showLO);

    connect(showCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Canvas::onShownChanelChange);

    return vlb;
}
