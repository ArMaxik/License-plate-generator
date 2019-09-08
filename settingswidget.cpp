#include "settingswidget.h"

static void RemoveLayout(QLayout* layout);

SettingsWidget::SettingsWidget(QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , emptyWidget(new QFrame(this))
    , settingsLayout(nullptr)
{
    setWidget(emptyWidget);
    emptyWidget->setLayout(new QVBoxLayout());
    emptyWidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
    emptyWidget->setLineWidth(2);
    setWindowTitle(tr("Settings"));

    show();
}

void SettingsWidget::SetSettingsLayout(AbstractModelItem *item)
{
    if(emptyWidget->layout() != nullptr) {
        setWidget(emptyWidget);
    }
    if(settingsLayout) {
        delete settingsLayout;
        settingsLayout = nullptr;
    }

    if(item != nullptr) {
        settingsLayout = item->getSettingsLayout();
//        widget->setLayout(item->getSettingsLayout());
        QFrame *w = new QFrame();
        w->setFrameStyle(QFrame::Panel | QFrame::Raised);
        w->setLineWidth(2);
        w->setLayout(settingsLayout);
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
