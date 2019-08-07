#include "textureeditorwidget.h"

#include <QVBoxLayout>
#include <QTabWidget>

TextureEditorWidget::TextureEditorWidget(QWidget *parent)
    : CustomCentralWidget(parent)
    , controllScene(new QGraphicsScene(this))
    , controllView(new ControllViewPanel(controllScene))
    , treeModel(new ItemsTreeModel(controllScene))
{
    connect(controllScene, &QGraphicsScene::selectionChanged,
            this, &TextureEditorWidget::onItemSelected);

    setUpLayout();
}

QGraphicsScene *TextureEditorWidget::getControllScene() const
{
    return controllScene;
}

ItemsTreeModel *TextureEditorWidget::getItemsTreeModel() const
{
    return treeModel;
}

void TextureEditorWidget::addStaticImageItem()
{
    treeModel->addItem(new StaticImageItem());
//    controllScene->addItem(new StaticImageItem());
}

void TextureEditorWidget::addStaticTextItem()
{
    treeModel->addItem(new StaticTextItem());
//    controllScene->addItem(new StaticTextItem());
}

void TextureEditorWidget::setUpLayout()
{
    QVBoxLayout *mainL = new QVBoxLayout();
    setLayout(mainL);

    QTabWidget *tw = new QTabWidget(this);
    mainL->addWidget(tw);

    QWidget *editTab = new QWidget();
    QVBoxLayout *etL = new QVBoxLayout();
    etL->addWidget(controllView, 0, Qt::AlignCenter);
    editTab->setLayout(etL);
    editTab->setBackgroundRole(QPalette::Dark);  // Сделай потом красивый серый цвет
    tw->addTab(editTab, tr("Edit"));

}

void TextureEditorWidget::onItemSelected()
{
    if(controllScene->selectedItems().empty()) {
        emit itemSelected(nullptr);
    } else {
        QGraphicsItem *hmm = controllScene->selectedItems()[ controllScene->selectedItems().count()-1];
        BasicItem *item = dynamic_cast<BasicItem*>(hmm);
        emit itemSelected(item);
    }
}
