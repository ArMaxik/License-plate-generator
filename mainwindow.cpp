#include "mainwindow.h"
#include "toolbar.h"
#include "settingswidget.h"
#include "itemsoverview.h"

#include <QAction>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , textureEdit(new TextureEditorWidget(this))
{
    setCentralWidget(textureEdit);

    setUpToolBar();
    setDockWidgets();

    resize(1280, 720);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUpToolBar()
{
    ToolBar *tb = new ToolBar(tr("Tool Bar"), this);

    connect(tb->getAddStaticImageAction(), &QAction::triggered,
            textureEdit, &TextureEditorWidget::addItem);



    addToolBar(tb);
}

void MainWindow::setDockWidgets()
{
    SettingsWidget *sw = new SettingsWidget();

    connect(textureEdit, &TextureEditorWidget::itemSelected,
            sw, &SettingsWidget::SetSettingsLayout);

    addDockWidget(Qt::LeftDockWidgetArea, sw);

    ItemsOverview *is = new ItemsOverview(textureEdit->getItemsTreeModel());

    connect(textureEdit, &TextureEditorWidget::itemSelected,
            is, &ItemsOverview::onSelectionChanged);

    addDockWidget(Qt::RightDockWidgetArea, is);
}
