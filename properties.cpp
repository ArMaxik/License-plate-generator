#include "properties.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QImage>
#include <QColorDialog>
// ========[ BasicPropertie ]==================================================

BasicPropertie::BasicPropertie(QString labelText)
    : QObject()
    , random(false)
    , label(labelText)
{

}

QLayout *BasicPropertie::getSettingsLayout() const
{
    QVBoxLayout *ml = new QVBoxLayout();

    QHBoxLayout *chbLO = new QHBoxLayout();
    QLabel *chbL = new QLabel(tr("Randomize"));
    QCheckBox *cb = new QCheckBox();

    cb->setCheckState(random ? Qt::Checked : Qt::Unchecked);

    chbLO->addWidget(chbL);
    chbLO->addWidget(cb);
    ml->addLayout(chbLO);

    return ml;
}

// ========[ NumberPropertie ]==================================================

NumberPropertie::NumberPropertie(QString labelText, int val, int min_v, int max_v)
    : BasicPropertie (labelText)
    , value(val)
    , min(min_v)
    , max(max_v)
    , numSB(new QSpinBox)
{
    if(min > max) {
        min = max;
    }
}

QLayout *NumberPropertie::getSettingsLayout() const
{
    QVBoxLayout *ml = static_cast<QVBoxLayout*>(BasicPropertie::getSettingsLayout());

    QHBoxLayout *numLO = new QHBoxLayout();
    QLabel *numL = new QLabel(label);

    numSB->setRange(min, max);
    numSB->setValue(value);

    numLO->addWidget(numL);
    numLO->addWidget(numSB);

    ml->addLayout(numLO);

    connect(numSB, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NumberPropertie::onValueChange);

//    QVBoxLayout *fl = new QVBoxLayout();

//    QFrame *frame = new QFrame();
//    frame->setLayout(ml);
//    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
//    frame->setLineWidth(1);

//    fl->addWidget(frame);

    return ml;
}

int NumberPropertie::getValue() const
{
    return value;
}

void NumberPropertie::setValue(int newValue)
{
    value = newValue;
    if(numSB != nullptr) {
        numSB->setValue(value);
    }
}

void NumberPropertie::setRange(int min, int max)
{
    this->min = min;
    this->max = max;
    if(numSB != nullptr) {
        numSB->setRange(min, max);
    }
}

void NumberPropertie::onValueChange(int newValue)
{
    if(newValue != value) {
        value = newValue;
        emit numberValueChange(value);
    }
}

// ========[ ImagePropertie ]==================================================

ImagePropertie::ImagePropertie(QString labelText)
    : BasicPropertie (labelText)

{

}

QLayout *ImagePropertie::getSettingsLayout() const
{
    QLayout *ml = BasicPropertie::getSettingsLayout();

    QHBoxLayout *pathLO = new QHBoxLayout();
    QLabel *pathL = new QLabel(label);
//    QLabel *pathLD = new QLabel();
    QPushButton *pathB = new QPushButton(tr("Select image"));

    connect(pathB, &QPushButton::clicked,
            this, &ImagePropertie::onPathButtonPush);

    pathLO->addWidget(pathL);
    pathLO->addWidget(pathB);
    ml->addItem(pathLO);

//    QVBoxLayout *fl = new QVBoxLayout();

//    QFrame *frame = new QFrame();
//    frame->setLayout(ml);
//    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
//    frame->setLineWidth(1);

//    fl->addWidget(frame);


    return ml;
}

QImage &ImagePropertie::getImage()
{
    return img;
}

void ImagePropertie::onPathButtonPush()
{
    QString path = QFileDialog::getOpenFileName(nullptr,
                                                 tr("Open Image"),
                                                 "/home/",
                                                 tr("Image Files (*.png *.jpg *.bmp)"));
    img = QImage(path);
    emit imageChange();
}

// ========[ StringPropertie ]==================================================

StringPropertie::StringPropertie(QString labelText, QString s)
    : BasicPropertie(labelText)
    , str(s)
{

}

QLayout *StringPropertie::getSettingsLayout() const
{
    QLayout *ml = BasicPropertie::getSettingsLayout();

    QHBoxLayout *strLO = new QHBoxLayout();
    QLabel *strL = new QLabel(label);
    QLineEdit *strLE = new QLineEdit(str);

    connect(strLE, &QLineEdit::textChanged,
            this, &StringPropertie::onStringChange);

    strLO->addWidget(strL);
    strLO->addWidget(strLE);
    ml->addItem(strLO);

    return ml;
}

QString &StringPropertie::getString()
{
    return str;
}

void StringPropertie::onStringChange(QString newStr)
{
    str = newStr;
    emit stringChange(newStr);
}

// ========[ ColorPropertie ]==================================================


ColorPropertie::ColorPropertie(QString labelText, QColor c)
    : BasicPropertie(labelText)
    , color(c)
{

}

QLayout *ColorPropertie::getSettingsLayout() const
{
    QLayout *ml = BasicPropertie::getSettingsLayout();

    QHBoxLayout *colorLO = new QHBoxLayout();
    QLabel *colorL = new QLabel(label);
    QPushButton *colorPB = new QPushButton(tr("Choose color"));

    connect(colorPB, &QPushButton::clicked,
            this, &ColorPropertie::onColorChange);

    colorLO->addWidget(colorL);
    colorLO->addWidget(colorPB);
    ml->addItem(colorLO);

    return ml;
}

QColor &ColorPropertie::getColor()
{
    return color;
}

void ColorPropertie::onColorChange()
{
    color = QColorDialog::getColor();
    emit colorChange(color);
}
