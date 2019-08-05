#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "textureeditorwidget.h"


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
    void setDockWidgets();

    TextureEditorWidget *textureEdit;
};

#endif // MAINWINDOW_H
