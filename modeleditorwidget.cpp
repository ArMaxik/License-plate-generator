#include "modeleditorwidget.h"
#include "managers.h"

#include <QVBoxLayout>
#include <QLabel>

ModelEditorWidget::ModelEditorWidget(QWidget *parent)
    : QWidget(parent)
    , treeModel(new SceneTreeModel())
{
    QVBoxLayout *mainL = new QVBoxLayout();
    setLayout(mainL);

    mainL->addWidget(new QLabel("Hi mark"));

    MaterialManager *mm = new MaterialManager(this);
    treeModel->addItem(mm);
}
