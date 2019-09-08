#ifndef SMARTLAYOUT_H
#define SMARTLAYOUT_H

#include <QVBoxLayout>
#include <QtDebug>

class SmartLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    SmartLayout(QWidget *parent = nullptr)
        : QVBoxLayout(parent) {}
    virtual ~SmartLayout() { clear(); }

    void clear();
};

class SmartLayoutHolder : public QObject
{
    Q_OBJECT
public:
    SmartLayoutHolder()
        : _layout(new SmartLayout())
    {
        connect(_layout, &QObject::destroyed,
                this, &SmartLayoutHolder::resetLayout);
    }

    SmartLayout *layout() { return _layout; }

private:
    SmartLayout *_layout;  // При уничтожении не создаст ли заново?

private slots:
    void resetLayout()
    {
        _layout = new SmartLayout();
        connect(_layout, &QObject::destroyed,
                this, &SmartLayoutHolder::resetLayout);
    }
};

#endif // SMARTLAYOUT_H
