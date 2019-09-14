#include "settingswidget.h"

static void RemoveLayout(QLayout* layout);

SettingsWidget::SettingsWidget(QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , emptyWidget(new QFrame(this))
    , settingsLayout(nullptr)
    , currentItem(nullptr)
{
    setWidget(emptyWidget);
    emptyWidget->setLayout(new QVBoxLayout());
    emptyWidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
    emptyWidget->setLineWidth(2);
    setWindowTitle(tr("Settings"));

    show();
}

void SettingsWidget::SetAbstractItem(AbstractModelItem *item)
{
    if(currentItem != nullptr) {
        disconnect(currentItem, &AbstractModelItem::layoutChanged,
                   this, &SettingsWidget::SetSettingsLayout);
    }
    currentItem = item;
    if(currentItem != nullptr) {
        connect(currentItem, &AbstractModelItem::layoutChanged,
                this, &SettingsWidget::SetSettingsLayout, Qt::QueuedConnection);
    }
    SetSettingsLayout();
}

void SettingsWidget::SetSettingsLayout()
{
    if(widget() != nullptr){
        RemoveLayout(widget()->layout());
    }
    if(emptyWidget->layout() != nullptr) {
        setWidget(emptyWidget);
    }
    if(currentItem == nullptr) {
        return;
    }
    QLayout* layout = currentItem->getSettingsLayout();

    QFrame *w = new QFrame();
    w->setFrameStyle(QFrame::Panel | QFrame::Raised);
    w->setLineWidth(2);
    w->setLayout(layout);
    setWidget(w);

}

static void RemoveLayout (QLayout* layout) // Эта вещь мне не нравится
{
    if (layout != nullptr)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr){
            if (QLayout* childLayout = item->layout()){
                RemoveLayout(childLayout);
            }
            else if (QWidget* widget = item->widget()){
                widget->hide();
                delete widget;
            }
            else delete item;
        }

        delete layout;
    }
}
