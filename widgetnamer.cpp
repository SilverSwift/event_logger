#include "widgetnamer.h"
#include <QApplication>
#include <QDebug>
#include <QWidget>

namespace {
    static const char* PropertyName = "id_name";
}

WidgetNamer::WidgetNamer(QObject *parent) : QObject(parent)
{
    qApp->installEventFilter(this);
}

bool WidgetNamer::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    if((event->type() == QEvent::ChildAdded)){
        this->inspect();
    }

return false;
}

void WidgetNamer::inspect()
{
    foreach (QWidget *parentWidget, QApplication::topLevelWidgets())
        this->nameWidget(parentWidget);
}

void WidgetNamer::nameWidget(QWidget *widget)
{
    QString name = widget->property(PropertyName).toString();
    if(name.isEmpty()){
        name = widget->metaObject()->className();
        name += QString::number(mCnt++);
        widget->setProperty(PropertyName, name);
        qDebug() << "Create name" <<name;
    }

    QWidgetList wgts = widget->findChildren<QWidget*>(QString(),
                                                      Qt::FindDirectChildrenOnly);

    foreach (QWidget* wgt, wgts)
        this->nameWidget(wgt);
}

void WidgetNamer::startWidgetNamer()
{
    WidgetNamer* widgetnamer = new WidgetNamer();

    widgetnamer->inspect();

    qApp->installEventFilter(widgetnamer);
}

