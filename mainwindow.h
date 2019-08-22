#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "textureeditorwidget.h"
#include "modeleditorwidget.h"
#include "itemsoverview.h"

#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setUpToolBar();
    void setUpDockWidgets();
    void setUpEditors();

    TextureEditorWidget *textureEdit;
    ModelEditorWidget *modelEdit;

    QItemSelectionModel *texS;
    QItemSelectionModel *modelS;

    ItemsOverview *itemsOverview;

private slots:
    void switchEditors();
};

#endif // MAINWINDOW_H
