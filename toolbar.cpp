#include "toolbar.h"

ToolBar::ToolBar(const QString &title, QWidget *parent)
    : QToolBar(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    addItemAction = new QAction(tr("Add Item"), parent);
    switchEditorsAction = new QAction(tr("Switch Editors"), parent);

    addAction(addItemAction);
    addAction(switchEditorsAction);
}

QAction *ToolBar::getAddItemAction() const
{
    return addItemAction;
}

QAction *ToolBar::getSwitchEditorsAction() const
{
    return  switchEditorsAction;
}
