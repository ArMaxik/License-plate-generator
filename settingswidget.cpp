#include "settingswidget.h"

static void RemoveLayout(QLayout* layout);

SettingsWidget::SettingsWidget(QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , widget(new QWidget(this))
{
    setWidget(widget);
    widget->setLayout(new QVBoxLayout());

    show();
}

void SettingsWidget::SetSettingsLayout(BasicItem *item)
{
    if(widget->layout() != nullptr) {
        RemoveLayout(widget->layout());
    }
    if(item != nullptr) {
        widget->setLayout(item->getSettingsLayout());
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
