#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "basicitem.h"

#include <QDockWidget>

class SettingsWidget :public QDockWidget
{
    Q_OBJECT
public:
    SettingsWidget(QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

public slots:
    void SetSettingsLayout(BasicItem *item);

private:
    QWidget *emptyWidget;
};

#endif // SETTINGSWIDGET_H
