#include "eventrecorder.h"
#include "fileserializer.h"
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWidget>

namespace {
    static const char* PropertyName = "id_name";
}

EventRecorder::EventRecorder(AbstractSerializer* serializer, QObject *parent) :
    AbstractRecorder(serializer, parent)
{
}

EventRecorder::~EventRecorder()
{
    this->stop();
}

void EventRecorder::start()
{
    if (mRecording)
        return;
    mRecording = true;
    mRuntime.restart();
    qApp->installEventFilter(this);
}

void EventRecorder::stop()
{
    if (!mRecording)
        return;
    mRecording = false;
    qApp->removeEventFilter(this);
    pSerializer->write(QJsonDocument(jsonArray).toJson());
}

QString EventRecorder::findParentObject(QObject *obj)
{
    if (!obj || !obj->parent() )
        return QString();

    QString head = this->findParentObject(obj->parent());
    if (!head.isEmpty())
        head += ".";
    head += obj->parent()->property(PropertyName).toString();

    return head;
}

void EventRecorder::inspect()
{
    foreach (QWidget *parentWidget, QApplication::topLevelWidgets())
        this->nameWidget(parentWidget);
}

QJsonObject EventRecorder::logEntry(QObject *watched, QEvent *event)
{
    QJsonObject eventObject;

    eventObject.insert("className", QJsonValue::fromVariant(watched->metaObject()->className()));
    eventObject.insert("objectName", QJsonValue::fromVariant(watched->property(PropertyName).toString()));
    eventObject.insert("eventType", QJsonValue::fromVariant(event->type()));
    eventObject.insert("timestamp", QJsonValue::fromVariant(mRuntime.elapsed()));

    eventObject.insert("parent", this->findParentObject(watched));
    qDebug()<<watched->property(PropertyName).toString();
    if (watched->parent())
        qDebug()<<watched->parent()->property(PropertyName).toString();

    return eventObject;
}

void EventRecorder::logInputEvent(QObject *watched, QEvent *event)
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
            QVariant modifier(qApp->keyboardModifiers());
            eventObject.insert("modifiers", QJsonValue::fromVariant(modifier));
            break;
        }

        default:
            qWarning()<<"unhandled event! error!";
            return;
    }

    jsonArray.append(eventObject);
}

void EventRecorder::nameWidget(QWidget *widget)
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

bool EventRecorder::eventFilter(QObject* watched, QEvent* event)
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
            return  false;

        default:
            qWarning()<<"unhandled event! error!";
            break;
    }

return false;
}
