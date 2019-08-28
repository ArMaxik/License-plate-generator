#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QLayout>
#include <QSpinBox>


class BasicPropertie : public QObject
{
    Q_OBJECT
public:
    BasicPropertie(QString labelText);

    virtual QLayout *getSettingsLayout() const;

signals:
    void changed();

protected:
    bool random;
    QString label;
};

class NumberPropertie : public BasicPropertie
{
    Q_OBJECT
public:
    NumberPropertie(QString labelText, int val, int min_v, int max_v);

    QLayout *getSettingsLayout() const override;

    int getValue() const;
    operator int() const { return value; }

public slots:
    void setValue(int newValue);
    void setRange(int min, int max);

signals:
    void numberValueChange(int newValue);

protected:
    int value;
    int min;
    int max;

    QSpinBox *numSB;

protected slots:
    void onValueChange(int newValue);
};

class ImagePropertie : public BasicPropertie
{
    Q_OBJECT
public:
    ImagePropertie(QString labelText);

    QLayout *getSettingsLayout() const override;
    QImage &getImage();
    operator QImage() const { return img; }

signals:
    void imageChange() const;

protected:
    QImage img;

protected slots:
    void onPathButtonPush();
};

class StringPropertie : public BasicPropertie
{
    Q_OBJECT
public:
    StringPropertie(QString labelText, QString s = "Text");

    QLayout *getSettingsLayout() const override;

    QString &getString();
    operator QString() const { return str; }

signals:
    void stringChange(QString newStr);

protected:
    QString str;

protected slots:
    void onStringChange(QString newStr);
};

class ColorPropertie : public BasicPropertie
{
    Q_OBJECT
public:
    ColorPropertie(QString labelText, QColor c = Qt::black);

    QLayout *getSettingsLayout() const override;

    QColor &getColor();
    operator QColor() const { return color; }

signals:
    void colorChange(QColor newColor);

protected:
    QColor color;

protected slots:
    void onColorChange();
};

#endif // PROPERTIES_H
