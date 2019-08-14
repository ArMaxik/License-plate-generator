#include "settingswidget.h"

static void RemoveLayout(QLayout* layout);

SettingsWidget::SettingsWidget(QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , emptyWidget(new QWidget(this))
{
    setWidget(emptyWidget);
    emptyWidget->setLayout(new QVBoxLayout());
    setWindowTitle(tr("Settings"));

    show();
}

void SettingsWidget::SetSettingsLayout(BasicItem *item)
{
    if(emptyWidget->layout() != nullptr) {
//        RemoveLayout(widget->layout());
//        delete this->widget();
        setWidget(emptyWidget);
    }
    if(item != nullptr) {
//        widget->setLayout(item->getSettingsLayout());
        QWidget *w = new QWidget();
        w->setLayout(item->getSettingsLayout());
        setWidget(w);
    }
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
