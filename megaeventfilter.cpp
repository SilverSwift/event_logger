#include "megaeventfilter.h"
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtWidgets>
#include <QStringBuilder>
#include <QGuiApplication>

namespace {
    static const char* PropertyName = "id_name";
}

MegaEventFilter::MegaEventFilter(QObject *parent) : QObject(parent)
{

    qApp->installEventFilter(this);
    connect(this, &MegaEventFilter::inspect,
            this, &MegaEventFilter::onInspect,
            Qt::QueuedConnection);
    emit inspect();
}

bool MegaEventFilter::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    if(     (event->type() != QEvent::MouseButtonPress) &&
            (event->type() != QEvent::KeyPress) &&
            (event->type() != QEvent::ChildAdded))
        return false;

    switch (event->type()) {
        case QEvent::KeyPress:
        case QEvent::MouseButtonPress:
            this->logInputEvent(watched, event);
            break;

        case QEvent::ChildAdded:
            emit inspect();
            return false;

        default:
            qWarning()<<"unhandled event! error!";
            break;
    }

    return false;
}

MegaEventFilter::~MegaEventFilter()
{
    this->saveJsonFile();
}

void MegaEventFilter::saveJsonFile()
{
    if(!jsonArray.isEmpty()){
        QFile file("EventFile.json");
        if (!file.open(QFile::WriteOnly))
            return;
        file.write(QJsonDocument(jsonArray).toJson());
        file.close();
    }
}

QString MegaEventFilter::findParentObject(QObject* obj)
{
    QString str;
    while(obj && obj->parent()){
        QString parentName = obj->property(PropertyName).toString();
        if(str.isEmpty())
            str = parentName;
        else
            str = parentName % QChar('.') % str;

        obj = obj->parent();
    }

    return str;
}

void MegaEventFilter::onInspect()
{
    foreach (QWidget *parentWidget, QApplication::topLevelWidgets())
        this->nameWidget(parentWidget);
}

void MegaEventFilter::logInputEvent(QObject *watched, QEvent *event)
{
    QJsonObject eventObject = this->logEntry(watched, event);

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            eventObject.insert("mouseButton", QJsonValue::fromVariant(mouseEvent->button()));
            eventObject.insert("x", QJsonValue::fromVariant(mouseEvent->x()));
            eventObject.insert("y", QJsonValue::fromVariant(mouseEvent->y()));
            break;
        }
        case QEvent::KeyPress: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            eventObject.insert("text", QJsonValue::fromVariant(keyEvent->key()));
            break;
        }

        default:
            qWarning()<<"unhandled event! error!";
            return;
    }

    QVariant modifier(qApp->keyboardModifiers());
    eventObject.insert("modifiers", QJsonValue::fromVariant(modifier));
    jsonArray.append(eventObject);
}

QJsonObject MegaEventFilter::logEntry(QObject* watched, QEvent* event)
{
    QJsonObject eventObject;

    eventObject.insert("className", QJsonValue::fromVariant(watched->metaObject()->className()));
    eventObject.insert("objectName", QJsonValue::fromVariant(watched->property(PropertyName).toString()));
    eventObject.insert("eventType", QJsonValue::fromVariant(event->type()));

    eventObject.insert("parent", this->findParentObject(watched));

    return eventObject;
}

void MegaEventFilter::nameWidget(QWidget* widget)
{
    QString name = widget->property(PropertyName).toString();
    if(name.isEmpty()){
        name = widget->metaObject()->className();
        name += QString::number(mCnt++);
        widget->setProperty(PropertyName, name);
    }

    QWidgetList wgts = widget->findChildren<QWidget*>(QString(),
                                                      Qt::FindDirectChildrenOnly);

    foreach (QWidget* wgt, wgts)
        this->nameWidget(wgt);

}

