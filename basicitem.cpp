#include "basicitem.h"

#include <QPainter>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>

#include <QDebug>

BasicItem::BasicItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , bound(0, 0, 50, 50)
    , name(tr("Unnamed Item"))
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}  // Что-то тут не то, хммм

void BasicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected()) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        painter->setPen(pen);

        painter->drawRect(boundingRect());
    }
}

QWidget *BasicItem::getSettingsWidget()
{
//    QVBoxLayout *vl = new QVBoxLayout();
    QGridLayout *gl = new QGridLayout();

    QLineEdit *nameLE = new QLineEdit(this->name);
    QLabel *nameL = new QLabel(tr("Items name"));

    bool isMovable = this->flags().testFlag(QGraphicsItem::ItemIsMovable);
    QCheckBox *movableCB = new QCheckBox();
    movableCB->setCheckState(isMovable ? Qt::Checked : Qt::Unchecked);

    QSpinBox *xSpinBox = new QSpinBox();
    QSpinBox *ySpinBox = new QSpinBox();
    xSpinBox->setRange(0, 5000);
    ySpinBox->setRange(0, 5000);
    xSpinBox->setValue(x());
    ySpinBox->setValue(y());
    xSpinBox->setEnabled(isMovable);
    ySpinBox->setEnabled(isMovable);

    QSpinBox *wSpinBox = new QSpinBox();
    QSpinBox *hSpinBox = new QSpinBox();
    wSpinBox->setRange(0, 5000);
    hSpinBox->setRange(0, 5000);
    wSpinBox->setValue(boundingRect().width());
    hSpinBox->setValue(boundingRect().height());

    QLabel *movL = new QLabel(tr("Movable"));
    QLabel *xposL = new QLabel(tr("X position"));
    QLabel *yposL = new QLabel(tr("Y position"));
    QLabel *widthL = new QLabel(tr("Width"));
    QLabel *heightL = new QLabel(tr("Height"));

    gl->addWidget(nameL, 0, 0);
    gl->addWidget(nameLE, 0, 1);
    gl->addWidget(movL, 1, 0);
    gl->addWidget(movableCB, 1, 1);
    gl->addWidget(xposL, 2, 0);
    gl->addWidget(xSpinBox, 2, 1);
    gl->addWidget(yposL, 3, 0);
    gl->addWidget(ySpinBox, 3, 1);
    gl->addWidget(widthL, 4, 0);
    gl->addWidget(wSpinBox, 4, 1);
    gl->addWidget(heightL, 5, 0);
    gl->addWidget(hSpinBox, 5, 1);

    // Name change
    connect(nameLE, &QLineEdit::textChanged,
            this, [this](QString text){ if(text.isEmpty()) text = tr("Unnamed Item"); name = text; });
    // Movable check
    connect(movableCB, &QCheckBox::stateChanged,
            this, [this](int state){ setFlag(QGraphicsItem::ItemIsMovable, state); });
    connect(movableCB, &QCheckBox::stateChanged,
            xSpinBox, &QSpinBox::setEnabled);
    connect(movableCB, &QCheckBox::stateChanged,
            ySpinBox, &QSpinBox::setEnabled);
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
    // spinBox size
    connect(wSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int w){ prepareGeometryChange(); bound.setWidth(w);});
    connect(hSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int h){ prepareGeometryChange(); bound.setHeight(h);});

    QWidget *wid = new QWidget();
    QVBoxLayout *ml = new QVBoxLayout();
    ml->addLayout(gl);
    wid->setLayout(ml);
    return  wid;
}

QString BasicItem::getName()
{
//    return QString("Hi");
//    qDebug() << name;
    return name;
}

QRectF BasicItem::boundingRect() const
{
    return bound;
}

StaticImageItem::StaticImageItem(QGraphicsItem *parent)
    : BasicItem (parent)
{

}

QWidget *StaticImageItem::getSettingsWidget()
{
    return BasicItem::getSettingsWidget();
}

void StaticImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
    painter->drawEllipse(boundingRect());
    BasicItem::paint(painter, option, widget);
}



StaticTextItem::StaticTextItem(QGraphicsItem *parent)
    : BasicItem(parent)
    , text(tr("Text"))
{
    font.setPixelSize(20);
}

QWidget *StaticTextItem::getSettingsWidget()
{
    QWidget *wid = BasicItem::getSettingsWidget();

    QLabel *tL = new QLabel(tr("Text"));
    QLineEdit *lE = new QLineEdit(text);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(tL);
    hl->addWidget(lE);

    static_cast<QVBoxLayout *>(wid->layout())->addLayout(hl);

    connect(lE, &QLineEdit::textChanged,
            this, [this](QString newText){ text = newText; });

    return wid;
}

void StaticTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setFont(font);
    painter->drawText(bound, text);
    prepareGeometryChange();
    QRectF r = painter->boundingRect(x(), y(), 500, 500, Qt::AlignLeft, text);
    bound.setWidth(r.width());
    bound.setHeight(r.height());
    BasicItem::paint(painter, option, widget);
}
