#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "modelitem.h"

#include <QDockWidget>
#include <QFrame>

class SettingsWidget :public QDockWidget
{
    Q_OBJECT
public:
    SettingsWidget(QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

public slots:
    void SetSettingsLayout(AbstractModelItem *item);

private:
    QFrame *emptyWidget;
    QLayout *settingsLayout;
};

#endif // SETTINGSWIDGET_H
