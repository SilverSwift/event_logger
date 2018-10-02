#include "eventrecorder.h"
#include "fileserializer.h"
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QMessageBox>

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

QJsonObject EventRecorder::logEntry(QObject *watched, QEvent *event)
{
    QJsonObject eventObject;

    eventObject.insert("className", QJsonValue::fromVariant(watched->metaObject()->className()));
    eventObject.insert("objectName", QJsonValue::fromVariant(watched->property(PropertyName).toString()));
    eventObject.insert("eventType", QJsonValue::fromVariant(event->type()));
    eventObject.insert("timestamp", QJsonValue::fromVariant(mRuntime.elapsed()));

    eventObject.insert("parent", this->findParentObject(watched));
    qDebug()<<this->findParentObject(watched) + "." +
              watched->property(PropertyName).toString();

    return eventObject;
}

void EventRecorder::logInputEvent(QObject *watched, QEvent *event)
{
    QJsonObject eventObject = this->logEntry(watched, event);

    switch (event->type()) {
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:{
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            eventObject.insert("mouseButton", QJsonValue::fromVariant(mouseEvent->button()));
            eventObject.insert("mouseButtons", QJsonValue::fromVariant(QVariant::fromValue(mouseEvent->buttons())));
            QVariant modifier(qApp->keyboardModifiers());
            eventObject.insert("modifiers", QJsonValue::fromVariant(modifier));
            eventObject.insert("x", QJsonValue::fromVariant(mouseEvent->x()));
            eventObject.insert("y", QJsonValue::fromVariant(mouseEvent->y()));
            break;
        }
        case QEvent::KeyPress:
        case QEvent::KeyRelease: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            eventObject.insert("text", QJsonValue::fromVariant(keyEvent->key()));
            QVariant modifier(qApp->keyboardModifiers());
            eventObject.insert("modifiers", QJsonValue::fromVariant(modifier));
            break;
        }

        default:
            return;
    }

    jsonArray.append(eventObject);
}

bool EventRecorder::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    switch (event->type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease: {
            qDebug()<<"Mouse click!";
            this->logInputEvent(watched, event);
            break;
        }

        default:

            break;
    }

    return false;
}
