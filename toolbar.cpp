#include "toolbar.h"

ToolBar::ToolBar(const QString &title, QWidget *parent)
    : QToolBar(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    addStaticImageAction = new QAction(tr("Add Static Image"), parent);
    addStaticTextAction = new QAction(tr("Add Static Text"), parent);

    addAction(addStaticImageAction);
    addAction(addStaticTextAction);
}

QAction *ToolBar::getAddStaticImageAction() const
{
    return addStaticImageAction;
}

QAction *ToolBar::getAddStaticTextAction() const
{
    return  addStaticTextAction;
}
