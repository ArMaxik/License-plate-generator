#include "toolbar.h"

ToolBar::ToolBar(const QString &title, QWidget *parent)
    : QToolBar(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    addStaticImageAction = new QAction(tr("Add Static Image Action"), parent);
    addAction(addStaticImageAction);
}

QAction *ToolBar::getAddStaticImageAction() const
{
    return addStaticImageAction;
}
