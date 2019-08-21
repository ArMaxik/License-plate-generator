#ifndef MODELEDITORWIDGET_H
#define MODELEDITORWIDGET_H

#include "treemodel.h"

#include <QWidget>

class ModelEditorWidget : public QWidget
{
public:
    ModelEditorWidget(QWidget *parent = nullptr);

    SceneTreeModel *getSceneTreeModel() const { return treeModel; }

private:
    SceneTreeModel *treeModel;
};

#endif // MODELEDITORWIDGET_H
