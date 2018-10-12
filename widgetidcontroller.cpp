#include "widgetidcontroller.h"
#include "eventconstants.h"
#include <QApplication>
#include <QDebug>
#include <QChildEvent>
#include <QWidget>
#include <QListWidget>

WidgetIdController::WidgetIdController(QObject *parent) : QObject(parent)
{
    this->inspect();
    qApp->installEventFilter(this);
}

bool WidgetIdController::sortByClassName(const QWidget* w1, const QWidget* w2)
{
    return w1->metaObject()->className() <= w2->metaObject()->className();
}

bool WidgetIdController::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    //if((event->type() == QEvent::ChildAdded)){
        if((event->type() == QEvent::ChildPolished)){
        auto child = static_cast<QChildEvent*>(event)->child();
        if(child->property(widgetId).toString().isEmpty() ){
            QString name = QString("%1%2")
                           .arg(child->metaObject()->className())
                           .arg(mCnt++);
            child->setProperty(widgetId, name);
        }
    }

    return false;
}

void WidgetIdController::inspect()
{
    QWidgetList parents = QApplication::topLevelWidgets();
    std::sort(parents.begin(), parents.end(), sortByClassName);

    foreach (QWidget* parent, parents)
        this->nameWidget(parent);
}

void WidgetIdController::nameWidget(QWidget *widget)
{
    QString name = widget->property(widgetId).toString();
    if(name.isEmpty()){
        name = widget->metaObject()->className();
        name += QString::number(mCnt++);
        widget->setProperty(widgetId, name);
        qDebug() << name;
    }


    QWidgetList wgts = widget->findChildren<QWidget*>(QString(),
                                                      Qt::FindDirectChildrenOnly);

    std::sort(wgts.begin(), wgts.end(), sortByClassName);

    foreach (QWidget* wgt, wgts)
        this->nameWidget(wgt);
}
