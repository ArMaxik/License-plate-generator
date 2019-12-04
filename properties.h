#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QLayout>
#include <QSpinBox>
#include <QXmlStreamWriter>


class BasicProperty : public QObject
{
    Q_OBJECT
public:
    BasicProperty(QString labelText);

    virtual QLayout *getSettingsLayout();

    virtual void toXml(QXmlStreamWriter &stream) = 0;


public slots:
    virtual void randomize();

signals:
    void changed();
    void layoutChanged();

protected:
    bool random;
    QString label;

    virtual void makeNotRandomLayout(QVBoxLayout *ml);
    virtual void makeRandomLayout(QVBoxLayout *ml);

    bool randoming;
};

class NumberProperty : public BasicProperty
{
    Q_OBJECT
public:
    NumberProperty(QString labelText, int val, int min_v, int max_v);

//    QLayout *getSettingsLayout() override;

    int getValue() const;
    operator int() const { return value; }
    void toXml(QXmlStreamWriter &stream) override;

public slots:
    void setValue(int newValue, bool silent = false);
    void setRange(int min, int max);
    virtual void randomize();


signals:
    void numberValueChange(int newValue);

protected:
    int value;
    int min;
    int max;

//    QSpinBox *numSB;

    virtual void makeNotRandomLayout(QVBoxLayout *ml) override;
    virtual void makeRandomLayout(QVBoxLayout *ml) override;


protected slots:
    void onValueChange(int newValue);
};

class ImageProperty : public BasicProperty
{
    Q_OBJECT
public:
    ImageProperty(QString labelText);

    QLayout *getSettingsLayout() override;
    QImage &getImage();
    operator QImage() const { return img; }

    void setImagePath(const QString &path);

    void toXml(QXmlStreamWriter &stream) override;

signals:
    void imageChange() const;

protected:
    QImage img;
    QString imgPath;

protected slots:
    void onPathButtonPush();
};

class StringProperty : public BasicProperty
{
    Q_OBJECT
public:
    StringProperty(QString labelText, QString s = "Text");

    QString &getString();
    operator QString() const { return str; }

    void toXml(QXmlStreamWriter &stream) override;

public slots:
    virtual void randomize();

signals:
    void stringChange(QString newStr);

protected:
    QString str;
    QString randomPattern;

    virtual void makeNotRandomLayout(QVBoxLayout *ml) override;
    virtual void makeRandomLayout(QVBoxLayout *ml) override;

    static const QString alphabet;
    static const QString numbers;

protected slots:
    void onStringChange(QString newStr);
};

class ColorProperty : public BasicProperty
{
    Q_OBJECT
public:
    ColorProperty(QString labelText, QColor c = Qt::black);

    QColor &getColor();
    operator QColor() const { return color; }

    void toXml(QXmlStreamWriter &stream) override;

signals:
    void colorChange(QColor newColor);

protected:
    QColor color;


    virtual void makeNotRandomLayout(QVBoxLayout *ml) override;
    virtual void makeRandomLayout(QVBoxLayout *ml) override;

protected slots:
    void onColorChange();
};

#endif // PROPERTIES_H
