#include "smartlayout.h"

#include <QWidget>

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

void SmartLayout::clear()
{
    QLayoutItem *item;
    while ((item = this->takeAt(0)) != nullptr){
        if (QLayout* childLayout = item->layout()){
            RemoveLayout(childLayout);
        }
        else if (QWidget* widget = item->widget()){
            widget->hide();
            delete widget;
        }
        else delete item;
    }
}
