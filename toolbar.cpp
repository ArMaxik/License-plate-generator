#include "toolbar.h"

ToolBar::ToolBar(const QString &title, QWidget *parent)
    : QToolBar(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    addItemAction = new QAction(tr("Add Item"), parent);
    switchEditorsAction = new QAction(tr("Switch Editors"), parent);
    randomizeAction = new QAction(tr("Randomize"), parent);
    clearAllAction = new QAction(tr("Clear All"), parent);
    saveSceneAction = new QAction(tr("Save Scene"), parent);
    loadSceneAction = new QAction(tr("Load Scene"), parent);

    addAction(addItemAction);
    addAction(switchEditorsAction);
    addAction(randomizeAction);
    addAction(clearAllAction);
    addAction(saveSceneAction);
    addAction(loadSceneAction);
}
