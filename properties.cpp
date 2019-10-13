#include "properties.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QImage>
#include <QColorDialog>
#include <QRandomGenerator>

#include <QDebug>

// ========[ BasicPropertie ]==================================================

BasicPropertie::BasicPropertie(QString labelText)
    : QObject()
    , random(false)
    , label(labelText)
{

}

QLayout *BasicPropertie::getSettingsLayout()
{
    QVBoxLayout *ml = new QVBoxLayout();

    QHBoxLayout *chbLO = new QHBoxLayout();
    QLabel *chbL = new QLabel(tr("Randomize"));
    QCheckBox *cb = new QCheckBox();

    cb->setCheckState(random ? Qt::Checked : Qt::Unchecked);

    chbLO->addWidget(chbL);
    chbLO->addWidget(cb);
    ml->addLayout(chbLO);

    connect(cb, &QCheckBox::stateChanged,
            this, [this](int state){ random = (state == Qt::Checked); emit layoutChanged(); });

    if(random) {
        makeRandomLayout(ml);
    } else {
        makeNotRandomLayout(ml);
    }

    return ml;
}

void BasicPropertie::randomize()
{

}

void BasicPropertie::makeNotRandomLayout(QVBoxLayout *ml)
{

}

void BasicPropertie::makeRandomLayout(QVBoxLayout *ml)
{

}

// ========[ NumberPropertie ]==================================================

NumberPropertie::NumberPropertie(QString labelText, int val, int min_v, int max_v)
    : BasicPropertie (labelText)
    , value(val)
    , min(min_v)
    , max(max_v)
//    , numSB(new QSpinBox)
{
    if(min > max) {
        min = max;
    }
}

void NumberPropertie::makeNotRandomLayout(QVBoxLayout *ml)
{
//    QVBoxLayout *ml = static_cast<QVBoxLayout*>(BasicPropertie::getSettingsLayout());

    QHBoxLayout *numLO = new QHBoxLayout();
    QLabel *numL = new QLabel(label);
    QSpinBox *numSB = new QSpinBox();

    numSB->setRange(min, max);
    numSB->setValue(value);

    numLO->addWidget(numL);
    numLO->addWidget(numSB);

    ml->addLayout(numLO);

    connect(numSB, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NumberPropertie::onValueChange);

    //    return ml;
}

void NumberPropertie::makeRandomLayout(QVBoxLayout *ml)
{
    QHBoxLayout *numLO = new QHBoxLayout();
    QLabel *numL = new QLabel(label);
    numLO->addWidget(numL);
    ml->addLayout(numLO);

    QHBoxLayout *numLOmin = new QHBoxLayout();
    QLabel *numLmin = new QLabel(tr("min value"));
    QSpinBox *numSBmin = new QSpinBox();
    numSBmin->setRange(1, INT_MAX);
    numSBmin->setValue(min);
    numLOmin->addWidget(numLmin);
    numLOmin->addWidget(numSBmin);

    QHBoxLayout *numLOmax = new QHBoxLayout();
    QLabel *numLmax = new QLabel(tr("max value"));
    QSpinBox *numSBmax = new QSpinBox();
    numSBmax->setRange(1, INT_MAX);
    numSBmax->setValue(max);
    numLOmax->addWidget(numLmax);
    numLOmax->addWidget(numSBmax);

    ml->addLayout(numLOmin);
    ml->addLayout(numLOmax);

    connect(numSBmin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int v){ min = v; numSBmax->setMinimum(v); });
    connect(numSBmax, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int v){ max = v; numSBmin->setMaximum(v); });
}

int NumberPropertie::getValue() const
{
    return value;
}

void NumberPropertie::setValue(int newValue, bool silent)
{
    value = newValue;
    if(!silent)
        emit changed();
    emit layoutChanged();
//    if(numSB != nullptr) {
//        numSB->setValue(value);
//    }
}

void NumberPropertie::setRange(int min, int max)
{
    this->min = min;
    this->max = max;
//    if(numSB != nullptr) {
//        numSB->setRange(min, max);
//    }
}

void NumberPropertie::randomize()
{
    if(random) {
        setValue(QRandomGenerator::global()->bounded(min, max));
        qDebug() << min << " " << max << " " << value;
    }
}

void NumberPropertie::onValueChange(int newValue)
{
    if(newValue != value) {
        value = newValue;
        emit changed();
    }
}

// ========[ ImagePropertie ]==================================================

ImagePropertie::ImagePropertie(QString labelText)
    : BasicPropertie (labelText)

{

}

QLayout *ImagePropertie::getSettingsLayout()
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

QLayout *StringPropertie::getSettingsLayout()
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

QLayout *ColorPropertie::getSettingsLayout()
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
    emit changed();
}
