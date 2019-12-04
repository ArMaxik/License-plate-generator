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
    , randoming(true)
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
    if(randoming) ml->addLayout(chbLO);

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
    randoming = false;
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

void StringProperty::makeNotRandomLayout(QVBoxLayout *ml)
{
    QHBoxLayout *strLO = new QHBoxLayout();
    QLabel *strL = new QLabel(label);
    QLineEdit *strLE = new QLineEdit(str);

    connect(strLE, &QLineEdit::textChanged,
            this, [this](QString newStr){ str = newStr; });
    connect(strLE, &QLineEdit::textChanged,
            this, &StringProperty::stringChange);

    strLO->addWidget(strL);
    strLO->addStretch();
    strLO->addWidget(strLE);
    ml->addItem(strLO);
}

void StringProperty::makeRandomLayout(QVBoxLayout *ml)
{
    QHBoxLayout *strLO = new QHBoxLayout();
    QLabel *strL = new QLabel(tr("Random pattern"));
    QLineEdit *strLE = new QLineEdit(randomPattern);

    connect(strLE, &QLineEdit::textChanged,
            this, [this](QString newStr){ randomPattern = newStr; });
    connect(strLE, &QLineEdit::textChanged,
            this, &StringProperty::stringChange);

    strLO->addWidget(strL);
    strLO->addStretch();
    strLO->addWidget(strLE);
    ml->addItem(strLO);
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

void StringProperty::randomize()
{
    str.clear();

    QRegularExpression re("(\\w)(\\d+)");
    QRegularExpressionMatchIterator i = re.globalMatch(randomPattern);
    QStringList words;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString code = match.captured(1);
        int num = match.captured(2).toInt();

        const QString *source;
        if(code == "L") {
            source = &alphabet;
        } else if(code == "D") {
            source = &numbers;
        } else {
            continue;
        }

        for(int i = 0; i < num; i++) {
            str += (*source)[QRandomGenerator::global()->bounded(source->size())];
        }
        qDebug() << match.captured(1) << match.captured(2);
    }
    emit stringChange(str);
}

void StringProperty::onStringChange(QString newStr)
{
    str = newStr;
    emit stringChange(newStr);
}

//const QString StringProperty::alphabet = QString("qwertyuiopasdfghjklzxcvbnm");
const QString StringProperty::alphabet = QString("ETOPAHKCBM");
const QString StringProperty::numbers = QString("0123456789");


// ========[ ColorPropertie ]==================================================


ColorProperty::ColorProperty(QString labelText, QColor c)
    : BasicProperty(labelText)
    , color(c)
{

}


void ColorProperty::makeNotRandomLayout(QVBoxLayout *ml)
{

    QHBoxLayout *colorLO = new QHBoxLayout();
    QLabel *colorL = new QLabel(label);
    QPushButton *colorPB = new QPushButton(tr("Choose color"));

    connect(colorPB, &QPushButton::clicked,
            this, &ColorProperty::onColorChange);

    colorLO->addWidget(colorL);
    colorLO->addWidget(colorPB);
    ml->addItem(colorLO);

}

void ColorProperty::makeRandomLayout(QVBoxLayout *ml)
{
    QHBoxLayout *numLO = new QHBoxLayout();
    QLabel *numL = new QLabel(label);
    numLO->addWidget(numL);
    ml->addLayout(numLO);

    QHBoxLayout *rLO = new QHBoxLayout();
    QLabel *rL = new QLabel(tr("Red"));
    ml->addWidget(rL);

    QHBoxLayout *rLOmin = new QHBoxLayout();
    QLabel *rLmin = new QLabel(tr("min value"));
    QSpinBox *rSBmin = new QSpinBox();
    rSBmin->setRange(1, INT_MAX);
    rSBmin->setValue(0);
    rLOmin->addWidget(rLmin);
    rLOmin->addWidget(rSBmin);

    QHBoxLayout *rLOmax = new QHBoxLayout();
    QLabel *rLmax = new QLabel(tr("max value"));
    QSpinBox *rSBmax = new QSpinBox();
    rSBmax->setRange(1, INT_MAX);
    rSBmax->setValue(255);
    rLOmax->addWidget(rLmax);
    rLOmax->addWidget(rSBmax);

    ml->addLayout(rLOmin);
    ml->addLayout(rLOmax);

    QLabel *gL = new QLabel(tr("Green"));
    ml->addWidget(gL);

    QHBoxLayout *gLOmin = new QHBoxLayout();
    QLabel *gLmin = new QLabel(tr("min value"));
    QSpinBox *gSBmin = new QSpinBox();
    gSBmin->setRange(1, INT_MAX);
    gSBmin->setValue(0);
    gLOmin->addWidget(gLmin);
    gLOmin->addWidget(gSBmin);

    QHBoxLayout *gLOmax = new QHBoxLayout();
    QLabel *gLmax = new QLabel(tr("max value"));
    QSpinBox *gSBmax = new QSpinBox();
    gSBmax->setRange(1, INT_MAX);
    gSBmax->setValue(255);
    gLOmax->addWidget(gLmax);
    gLOmax->addWidget(gSBmax);

    ml->addLayout(gLOmin);
    ml->addLayout(gLOmax);

    QLabel *bL = new QLabel(tr("Blue"));
    ml->addWidget(bL);

    QHBoxLayout *bLOmin = new QHBoxLayout();
    QLabel *bLmin = new QLabel(tr("min value"));
    QSpinBox *bSBmin = new QSpinBox();
    bSBmin->setRange(1, INT_MAX);
    bSBmin->setValue(0);
    bLOmin->addWidget(bLmin);
    bLOmin->addWidget(bSBmin);

    QHBoxLayout *bLOmax = new QHBoxLayout();
    QLabel *bLmax = new QLabel(tr("max value"));
    QSpinBox *bSBmax = new QSpinBox();
    bSBmax->setRange(1, INT_MAX);
    bSBmax->setValue(255);
    bLOmax->addWidget(bLmax);
    bLOmax->addWidget(bSBmax);

    ml->addLayout(bLOmin);
    ml->addLayout(bLOmax);
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
