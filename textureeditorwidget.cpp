#include "textureeditorwidget.h"

#include <QVBoxLayout>
#include <QTabWidget>

TextureEditorWidget::TextureEditorWidget(QWidget *parent)
    : CustomCentralWidget(parent)
    , treeModel(new ItemsTreeModel())
    , controllView(new ViewWidget())
    , diffuseView(new ViewWidget())
    , specularView(new ViewWidget())
    , normalView(new ViewWidget())
{
    connect(controllView->getScene(), &QGraphicsScene::selectionChanged,
            this, &TextureEditorWidget::onItemSelected);

    Canvas *canvas = new Canvas(QSize(600, 400));
    controllView->addItem(canvas);

    treeModel->setCanvas(canvas);
    controllView->setCanvas(canvas);
    diffuseView->setCanvas(canvas);
    specularView->setCanvas(canvas);

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

void TextureEditorWidget::addItem()
{
    BasicItem *item = new BasicItem();
    treeModel->addItem(item);

    controllView->addItem(item);
    diffuseView->addItem(item->getDiffuseChanel());
    specularView->addItem(item->getSpecularChanel());
}

void TextureEditorWidget::setUpLayout()
{
    QVBoxLayout *mainL = new QVBoxLayout();
    setLayout(mainL);

    QTabWidget *tw = new QTabWidget(this);
    mainL->addWidget(tw);

    tw->addTab(controllView, tr("Edit"));
    tw->addTab(diffuseView, tr("Diffuse chanel"));
    tw->addTab(specularView, tr("Specular chanel"));
}

void TextureEditorWidget::onItemSelected()
{
    if(controllView->getScene()->selectedItems().empty()) {
        emit itemSelected(nullptr);
    } else {
        QGraphicsItem *hmm = controllView->getScene()->selectedItems().takeLast();
        BasicItem *item = dynamic_cast<BasicItem*>(hmm);
        emit itemSelected(item);
    }
}
