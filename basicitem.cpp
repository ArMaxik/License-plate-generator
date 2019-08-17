#include "basicitem.h"

#include <QPainter>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QFrame>

#include <QDebug>

BasicItem::BasicItem()
    : QGraphicsObject()
    , name(QObject::tr("Unnamed Item"))
    , bound(new BoundRect(QRectF(0.0, 0.0, 50.0, 50.0), this))
    , diffuseCh(new DiffuseChanel(bound))
    , specularCh(new SpecularChanel(bound))
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    diffuseCh->setParentItem(this);
    specularCh->setParentItem(this);
}

void BasicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    diffuseCh->paint(painter, option, widget);

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

BoundRect *BasicItem::getBoundRect() const
{
    return bound;
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

QLayout *BasicItem::setUpBasicLayout()
{
    QVBoxLayout *vlb = new QVBoxLayout();

    QLineEdit *nameLE = new QLineEdit(this->name);
    QLabel *nameL = new QLabel(tr("Items name"));

    QSpinBox *xSpinBox = new QSpinBox();
    QSpinBox *ySpinBox = new QSpinBox();
    xSpinBox->setRange(0, 5000);
    ySpinBox->setRange(0, 5000);
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

    return vlb;
}

void BasicItem::onChanelSizeChange()
{
    emit sizeChanged();
}

