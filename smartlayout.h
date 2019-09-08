#ifndef SMARTLAYOUT_H
#define SMARTLAYOUT_H

#include <QVBoxLayout>

class SmartLayout : public QVBoxLayout
{
public:
    SmartLayout(QWidget *parent = nullptr)
        : QVBoxLayout(parent) {}
    virtual ~SmartLayout() { clear(); }

    void clear();

};

#endif // SMARTLAYOUT_H
