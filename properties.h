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

#endif // PROPERTIES_H
