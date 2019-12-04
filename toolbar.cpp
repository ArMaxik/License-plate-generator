#include "toolbar.h"
#include <QToolButton>

ToolBar::ToolBar(const QString &title, QWidget *parent)
    : QToolBar(parent)
    , actionsVisible(true)
{
    setWindowTitle(title);
    setObjectName(title);

    addItemAction = new QAction(tr("Add Item"), parent);
    addItemAction->setIcon(QIcon(":/icons/icons/add.png"));
    QToolButton *addB = new QToolButton();
    addB->setDefaultAction(addItemAction);
    addB->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    switchEditorsAction = new QAction(tr("Switch Editors"), parent);
    switchEditorsAction->setIcon(QIcon(":/icons/icons/switch.png"));
    QToolButton *switchB = new QToolButton();
    switchB->setDefaultAction(switchEditorsAction);
    switchB->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    randomizeAction = new QAction(tr("Randomize"), parent);
    randomizeAction->setIcon(QIcon(":/icons/icons/randomize.png"));
    QToolButton *randomizeB = new QToolButton();
    randomizeB->setDefaultAction(randomizeAction);
    randomizeB->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    clearAllAction = new QAction(tr("Clear All"), parent);
    clearAllAction->setIcon(QIcon(":/icons/icons/clear.png"));
    QToolButton *clearB = new QToolButton();
    clearB->setDefaultAction(clearAllAction);
    clearB->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    saveSceneAction = new QAction(tr("Save Scene"), parent);
    saveSceneAction->setIcon(QIcon(":/icons/icons/save.png"));
    QToolButton *saveB = new QToolButton();
    saveB->setDefaultAction(saveSceneAction);
    saveB->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    loadSceneAction = new QAction(tr("Load Scene"), parent);
    loadSceneAction->setIcon(QIcon(":/icons/icons/open.png"));
    QToolButton *loadB = new QToolButton();
    loadB->setDefaultAction(loadSceneAction);
    loadB->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    addWidget(switchB);
    addSeparator();
    addWidget(addB);
    addWidget(randomizeB);
    addWidget(clearB);
    addWidget(saveB);
    addWidget(loadB);

    connect(switchEditorsAction, &QAction::triggered,
            this, &ToolBar::toggleEditors);
}

void ToolBar::toggleEditors()
{
    actionsVisible = !actionsVisible;

    addItemAction->setVisible(actionsVisible);
    randomizeAction->setVisible(actionsVisible);
    clearAllAction->setVisible(actionsVisible);
    saveSceneAction->setVisible(actionsVisible);
    loadSceneAction->setVisible(actionsVisible);
}
