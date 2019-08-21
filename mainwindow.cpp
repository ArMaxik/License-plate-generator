#include "mainwindow.h"
#include "toolbar.h"
#include "settingswidget.h"

#include <QAction>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , textureEdit(new TextureEditorWidget(this))
    , modelEdit(new ModelEditorWidget(this))
{
    setUpEditors();
    setUpToolBar();
    setUpDockWidgets();

    resize(1280, 720);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUpToolBar()
{
    ToolBar *tb = new ToolBar(tr("Tool Bar"), this);

    connect(tb->getAddItemAction(), &QAction::triggered,
            textureEdit, &TextureEditorWidget::addItem);

    connect(tb->getSwitchEditorsAction(), &QAction::triggered,
            this, &MainWindow::switchEditors);



    addToolBar(tb);
}

void MainWindow::setUpDockWidgets()
{
    SettingsWidget *sw = new SettingsWidget();


    addDockWidget(Qt::LeftDockWidgetArea, sw);

    itemsOverview = new ItemsOverview(textureEdit->getItemsTreeModel());

    connect(textureEdit, &TextureEditorWidget::itemSelected,
            itemsOverview, &ItemsOverview::onSelectionChanged);

    connect(itemsOverview, &ItemsOverview::itemSelected,
            sw, &SettingsWidget::SetSettingsLayout);

    addDockWidget(Qt::RightDockWidgetArea, itemsOverview);
}

void MainWindow::setUpEditors()
{
    modelEdit->hide();
    QVBoxLayout *l = new QVBoxLayout();
    l->setMargin(0);
    l->addWidget(textureEdit);
    l->addWidget(modelEdit);
    QWidget *cw = new QWidget(this);
    cw->setLayout(l);
    setCentralWidget(cw);
}

void MainWindow::switchEditors()
{
    bool isTextureEditorVisible = !textureEdit->isVisible();
    textureEdit->setVisible(isTextureEditorVisible);
    modelEdit->setVisible(!isTextureEditorVisible);

    if(isTextureEditorVisible) {
        itemsOverview->setModel(textureEdit->getItemsTreeModel());
    } else {
        itemsOverview->setModel(modelEdit->getSceneTreeModel());
    }
}
