#include "properties.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

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
//    QSpinBox *numSB = new QSpinBox();

    numSB->setRange(min, max);
    numSB->setValue(value);

    numLO->addWidget(numL);
    numLO->addWidget(numSB);

    ml->addLayout(numLO);

    connect(numSB, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NumberPropertie::onValueChange);

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

ImagePropertie::ImagePropertie(QString labelText)
    : BasicPropertie (labelText)
{

}

QLayout *ImagePropertie::getSettingsLayout() const
{
    QVBoxLayout *ml = static_cast<QVBoxLayout*>(BasicPropertie::getSettingsLayout());

    QHBoxLayout *pathLO = new QHBoxLayout();
    QLabel *pathL = new QLabel(label);
    QLabel *pathLD = new QLabel();
    QPushButton *pathB = new QPushButton(tr("Select image"));

    connect(pathB, &QPushButton::clicked,
            this, [=]() {
        pathLD->setText(QFileDialog::getOpenFileName(nullptr,
                                                     tr("Open Image"),
                                                     "/home/",
                                                     tr("Image Files (*.png *.jpg *.bmp)")));
        emit imagePathChange(pathLD->text());
    });

    pathLO->addWidget(pathL);
    pathLO->addWidget(pathLD);
    pathLO->addWidget(pathB);
    ml->addLayout(pathLO);

    return ml;
}

void ImagePropertie::onPathButtonPush()
{
//    savePath->setText(
//                    QFileDialog::getExistingDirectory(
//                               this
//                             , "Save path"
//                             , ""
//                             , QFileDialog::ShowDirsOnly
//                             | QFileDialog::DontResolveSymlinks) + "/");
}

void ImagePropertie::onImgPathChange(const QString &text)
{
    imgPath = text;
}

