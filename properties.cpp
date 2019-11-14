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

BasicProperty::BasicProperty(QString labelText)
    : QObject()
    , random(false)
    , label(labelText)
{

}

QLayout *BasicProperty::getSettingsLayout()
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

void BasicProperty::randomize()
{

}

void BasicProperty::makeNotRandomLayout(QVBoxLayout *ml)
{

}

void BasicProperty::makeRandomLayout(QVBoxLayout *ml)
{

}

// ========[ NumberPropertie ]==================================================

NumberProperty::NumberProperty(QString labelText, int val, int min_v, int max_v)
    : BasicProperty (labelText)
    , value(val)
    , min(min_v)
    , max(max_v)
//    , numSB(new QSpinBox)
{
    if(min > max) {
        min = max;
    }
}

void NumberProperty::makeNotRandomLayout(QVBoxLayout *ml)
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
            this, &NumberProperty::onValueChange);

    //    return ml;
}

void NumberProperty::makeRandomLayout(QVBoxLayout *ml)
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

int NumberProperty::getValue() const
{
    return value;
}

void NumberProperty::toXml(QXmlStreamWriter &stream)
{
    stream.writeStartElement("NumberPropertie");
    stream.writeAttribute("label", label);
    stream.writeTextElement("value", QString::number(value));
    stream.writeTextElement("min", QString::number(min));
    stream.writeTextElement("max", QString::number(max));
    stream.writeEndElement();
}

void NumberProperty::setValue(int newValue, bool silent)
{
    value = newValue;
    if(!silent)
        emit changed();
    emit layoutChanged();
//    if(numSB != nullptr) {
//        numSB->setValue(value);
//    }
}

void NumberProperty::setRange(int min, int max)
{
    this->min = min;
    this->max = max;
//    if(numSB != nullptr) {
//        numSB->setRange(min, max);
//    }
}

void NumberProperty::randomize()
{
    if(random) {
        setValue(QRandomGenerator::global()->bounded(min, max));
//        qDebug() << min << " " << max << " " << value;
    }
}

void NumberProperty::onValueChange(int newValue)
{
    if(newValue != value) {
        value = newValue;
        emit changed();
    }
}

// ========[ ImagePropertie ]==================================================

ImageProperty::ImageProperty(QString labelText)
    : BasicProperty (labelText)

{

}

QLayout *ImageProperty::getSettingsLayout()
{
    QLayout *ml = BasicProperty::getSettingsLayout();

    QHBoxLayout *pathLO = new QHBoxLayout();
    QLabel *pathL = new QLabel(label);
//    QLabel *pathLD = new QLabel();
    QPushButton *pathB = new QPushButton(tr("Select image"));

    connect(pathB, &QPushButton::clicked,
            this, &ImageProperty::onPathButtonPush, Qt::QueuedConnection);

    pathLO->addWidget(pathL);
    pathLO->addWidget(pathB);
    ml->addItem(pathLO);
    QLabel *imgL = new QLabel();
    imgL->setPixmap(QPixmap::fromImage(img).scaledToWidth(150, Qt::SmoothTransformation));
    ml->addWidget(imgL);

//    QVBoxLayout *fl = new QVBoxLayout();

//    QFrame *frame = new QFrame();
//    frame->setLayout(ml);
//    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
//    frame->setLineWidth(1);

//    fl->addWidget(frame);


    return ml;
}

QImage &ImageProperty::getImage()
{
    return img;
}

void ImageProperty::setImagePath(const QString &path)
{
    imgPath = path;
    img = QImage(imgPath);
    emit imageChange();
}

void ImageProperty::toXml(QXmlStreamWriter &stream)
{
    stream.writeStartElement("ImagePropertie");
    stream.writeAttribute("label", label);
    stream.writeTextElement("imgPath", imgPath);
    stream.writeEndElement();
}

void ImageProperty::onPathButtonPush()
{
    imgPath = QFileDialog::getOpenFileName(nullptr,
                                                 tr("Open Image"),
                                                 "/home/",
                                                 tr("Image Files (*.png *.jpg *.bmp)"));

//    QFileDialog fileDialog(nullptr,
//                           tr("Open Image"),
//                           "/home/",
//                           tr("Image Files (*.png *.jpg *.bmp)"));
//    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
//    fileDialog.setFileMode(QFileDialog::ExistingFiles);
//    fileDialog.setOption(QFileDialog::DontUseNativeDialog,true);
//    if (QDialog::Accepted != fileDialog.exec())
//        return;

//    imgPath = fileDialog.selectedFiles().first();
    img = QImage(imgPath);
    emit imageChange();
}

// ========[ StringPropertie ]==================================================

StringProperty::StringProperty(QString labelText, QString s)
    : BasicProperty(labelText)
    , str(s)
{

}

QLayout *StringProperty::getSettingsLayout()
{
    QLayout *ml = BasicProperty::getSettingsLayout();

    QHBoxLayout *strLO = new QHBoxLayout();
    QLabel *strL = new QLabel(label);
    QLineEdit *strLE = new QLineEdit(str);

    connect(strLE, &QLineEdit::textChanged,
            this, &StringProperty::onStringChange);

    strLO->addWidget(strL);
    strLO->addWidget(strLE);
    ml->addItem(strLO);

    return ml;
}

QString &StringProperty::getString()
{
    return str;
}

void StringProperty::toXml(QXmlStreamWriter &stream)
{
    stream.writeStartElement("StringPropertie");
    stream.writeAttribute("label", label);
    stream.writeTextElement("str", str);
    stream.writeEndElement();
}

void StringProperty::onStringChange(QString newStr)
{
    str = newStr;
    emit stringChange(newStr);
}

// ========[ ColorPropertie ]==================================================


ColorProperty::ColorProperty(QString labelText, QColor c)
    : BasicProperty(labelText)
    , color(c)
{

}

QLayout *ColorProperty::getSettingsLayout()
{
    QLayout *ml = BasicProperty::getSettingsLayout();

    QHBoxLayout *colorLO = new QHBoxLayout();
    QLabel *colorL = new QLabel(label);
    QPushButton *colorPB = new QPushButton(tr("Choose color"));

    connect(colorPB, &QPushButton::clicked,
            this, &ColorProperty::onColorChange);

    colorLO->addWidget(colorL);
    colorLO->addWidget(colorPB);
    ml->addItem(colorLO);

    return ml;
}

QColor &ColorProperty::getColor()
{
    return color;
}

void ColorProperty::toXml(QXmlStreamWriter &stream)
{
    stream.writeStartElement("ColorPropertie");
    stream.writeAttribute("label", label);
    stream.writeTextElement("red", QString::number(color.red()));
    stream.writeTextElement("green", QString::number(color.green()));
    stream.writeTextElement("blue", QString::number(color.blue()));
    stream.writeEndElement();
}

void ColorProperty::onColorChange()
{
    color = QColorDialog::getColor();
    emit changed();
}
