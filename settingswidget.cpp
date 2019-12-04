#include "settingswidget.h"

static void RemoveLayout(QLayout* layout);

SettingsWidget::SettingsWidget(QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , emptyWidget(new QFrame(this))
    , settingsLayout(nullptr)
    , currentItem(nullptr)
    , scrollArea(new QScrollArea())
{
    setWidget(scrollArea);
    scrollArea->setWidget(emptyWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);

    emptyWidget->setLayout(new QVBoxLayout());
    emptyWidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
    emptyWidget->setLineWidth(2);
    emptyWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
    emptyWidget->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
    setMinimumWidth(260);
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
        RemoveLayout(emptyWidget->layout());
    }
    if(currentItem == nullptr) {
        return;
    }
    QLayout* layout = currentItem->getSettingsLayout();

//    QScrollArea *sa =new QScrollArea();
    //sa->setWidget(w);
    emptyWidget->setLayout(layout);
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
