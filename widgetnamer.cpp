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

    if((event->type() == QEvent::ChildAdded))
        this->inspect();


    return false;
}

void WidgetNamer::inspect()
{
    auto parents = QApplication::topLevelWidgets();
    foreach (auto parent, parents)
        this->nameWidget(parent);
}

void WidgetNamer::nameWidget(QWidget *widget, int tab)
{
    QString name = widget->property(PropertyName).toString();
    if(name.isEmpty()){
        name = widget->metaObject()->className();
        name += QString::number(mCnt++);
        widget->setProperty(PropertyName, name);
        qDebug() <<QString("%1%2").arg(QString(tab, ' ')).arg(name);
    }

    QWidgetList wgts = widget->findChildren<QWidget*>(QString(),
                                                      Qt::FindDirectChildrenOnly);

    foreach (QWidget* wgt, wgts)
        this->nameWidget(wgt, tab + 1);
}

void WidgetNamer::startWidgetNamer()
{
    WidgetNamer* widgetnamer = new WidgetNamer();

    widgetnamer->inspect();

    qApp->installEventFilter(widgetnamer);
}

