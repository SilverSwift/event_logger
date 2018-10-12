#include "widgetnamer.h"
#include "eventconstants.h"
#include <QApplication>
#include <QDebug>
#include <QChildEvent>
#include <QWidget>

WidgetNamer::WidgetNamer(QObject *parent) : QObject(parent)
{
    this->inspect();
    qApp->installEventFilter(this);
}

bool WidgetNamer::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    if((event->type() == QEvent::ChildAdded)){
        auto child = static_cast<QChildEvent*>(event)->child();
        QString name = QString("%1%2")
                           .arg(child->metaObject()->className())
                           .arg(mCnt++);
        child->setProperty(widgetId, name);
    }

    return false;
}

void WidgetNamer::inspect()
{
    auto parents = QApplication::topLevelWidgets();
    foreach (auto parent, parents)
        this->nameWidget(parent);
}

void WidgetNamer::nameWidget(QWidget *widget)
{
    QString name = widget->property(widgetId).toString();
    if(name.isEmpty()){
        name = widget->metaObject()->className();
        name += QString::number(mCnt++);
        widget->setProperty(widgetId, name);
    }

    QWidgetList wgts = widget->findChildren<QWidget*>(QString(),
                                                      Qt::FindDirectChildrenOnly);

    foreach (QWidget* wgt, wgts)
        this->nameWidget(wgt);
}
