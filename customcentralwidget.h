#ifndef CUSTOMCENTRALWIDGET_H
#define CUSTOMCENTRALWIDGET_H

#include <QWidget>

class CustomCentralWidget : public QWidget
{
    Q_OBJECT
public:
    CustomCentralWidget(QWidget *parent = nullptr);

signals:
    void ItemSelected(); // TODO: to-do

public slots:

};

#endif // CUSTOMCENTRALWIDGET_H
