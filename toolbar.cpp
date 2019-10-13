#include "toolbar.h"

ToolBar::ToolBar(const QString &title, QWidget *parent)
    : QToolBar(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    addItemAction = new QAction(tr("Add Item"), parent);
    switchEditorsAction = new QAction(tr("Switch Editors"), parent);
    randomizeAction = new QAction(tr("Randomize"), parent);

    addAction(addItemAction);
    addAction(switchEditorsAction);
    addAction(randomizeAction);
}

QAction *ToolBar::getAddItemAction() const
{
    return addItemAction;
}

QAction *ToolBar::getSwitchEditorsAction() const
{
    return  switchEditorsAction;
}

QAction *ToolBar::getrandomizeAction() const
{
    return randomizeAction;
}
