#include "basicitem.h"

#include <QPainter>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QFrame>
#include <QComboBox>

#include <QDebug>

BasicItem::BasicItem()
    : QGraphicsObject()
    , name(QObject::tr("Unnamed Item"))
    , bound(new BoundRect(QRectF(0.0, 0.0, 0.0, 0.0), this))
    , diffuseCh(new DiffuseChanel(bound))
    , specularCh(new SpecularChanel(bound))
    , shownC(Chanels::diffuseC)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    diffuseCh->setAffectSize(true);

    connect(this, &QGraphicsObject::xChanged,
            this, [this](){ diffuseCh->setX(x()); });

    connect(this, &QGraphicsObject::yChanged,
            this, [this](){ diffuseCh->setY(y()); });

    connect(this, &QGraphicsObject::xChanged,
            this, [this](){ specularCh->setX(x()); });

    connect(this, &QGraphicsObject::yChanged,
            this, [this](){ specularCh->setY(y()); });
}

void BasicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (shownC) {
    case Chanels::diffuseC:
        diffuseCh->paint(painter, option, widget);
        break;
    case Chanels::specularC:
        specularCh->paint(painter, option, widget);
        break;
    case Chanels::normalC:
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
    QVBoxLayout *mainL = new QVBoxLayout();

    QGroupBox *basicGroup = new QGroupBox(tr("Basic properties"));
    QGroupBox *diffuseGroup = new QGroupBox(tr("Diffuse chanel"));
    QGroupBox *specularGroup = new QGroupBox(tr("Specular chanel"));

    basicGroup->setLayout(setUpBasicLayout());
    diffuseGroup->setLayout(diffuseCh->getSettingsLayout());
    specularGroup->setLayout(specularCh->getSettingsLayout());

    mainL->addWidget(basicGroup);
    mainL->addWidget(diffuseGroup);
    mainL->addWidget(specularGroup);

    return mainL;
}

QString BasicItem::getName()
{
    return name;
}

BasicChanel *BasicItem::getDiffuseChanel()
{
    return diffuseCh;
}

BasicChanel *BasicItem::getSpecularChanel()
{
    return specularCh;
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

    QLabel *xposL = new QLabel(tr("X position"));
    QLabel *yposL = new QLabel(tr("Y position"));

    QHBoxLayout *nameLt = new QHBoxLayout();
    QHBoxLayout *xPosLt = new QHBoxLayout();
    QHBoxLayout *yPosLt = new QHBoxLayout();

    vlb->addLayout(nameLt);
    vlb->addLayout(xPosLt);
    vlb->addLayout(yPosLt);

    nameLt->addWidget(nameL);
    nameLt->addWidget(nameLE);

    xPosLt->addWidget(xposL);
    xPosLt->addWidget(xSpinBox);

    yPosLt->addWidget(yposL);
    yPosLt->addWidget(ySpinBox);

    // Name change
    connect(nameLE, &QLineEdit::textChanged,
            this, [this](QString text){ if(text.isEmpty()) text = tr("Unnamed Item"); name = text; });
    // Position spinBox affect on object
    connect(xSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int x){ setX(x); });
    connect(ySpinBox,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int y){ setY(y); });
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
//    showCB->addItem(tr("Normal"));
    showCB->setCurrentIndex(shownC);

    showLO->addWidget(showL);
    showLO->addWidget(showCB);
    vlb->addLayout(showLO);

    connect(showCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BasicItem::onShownChanelChange);

    return vlb;
}

void BasicItem::onChanelSizeChange()
{
    emit sizeChanged();
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


Canvas::Canvas(QSize size)
    : BasicItem ()
{
    setFlag(QGraphicsItem::ItemIsMovable, false);

    diffuseCh->setAffectSize(false);

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
        break;
    }
}

QLayout *Canvas::getSettingsLayout()
{
    QVBoxLayout *mainL = new QVBoxLayout();

    QGroupBox *basicGroup = new QGroupBox(tr("Basic properties"));
    QGroupBox *diffuseGroup = new QGroupBox(tr("Diffuse chanel"));
    QGroupBox *specularGroup = new QGroupBox(tr("Specular chanel"));

    basicGroup->setLayout(setUpBasicLayout());
    diffuseGroup->setLayout(diffuseCh->getSettingsLayout());
    specularGroup->setLayout(specularCh->getSettingsLayout());

    mainL->addWidget(basicGroup);
    mainL->addWidget(diffuseGroup);
    mainL->addWidget(specularGroup);

    return mainL;
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
            this, [this](int w){ bound->setWidth(w); });
    connect(xSpinBox,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int h){ bound->setHeight(h); });

    // Choose what chanel to show
    QHBoxLayout *showLO = new QHBoxLayout();
    QLabel *showL = new QLabel(tr("Show chanel in edit"));
    QComboBox *showCB = new QComboBox();

    showCB->addItem(tr("Diffuse"));
    showCB->addItem(tr("Specular"));
//    showCB->addItem(tr("Normal"));
    showCB->setCurrentIndex(shownC);

    showLO->addWidget(showL);
    showLO->addWidget(showCB);
    vlb->addLayout(showLO);

    connect(showCB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Canvas::onShownChanelChange);

    return vlb;
}
