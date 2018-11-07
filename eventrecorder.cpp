#include "eventrecorder.h"
#include "eventconstants.h"
#include "fileserializer.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QEnterEvent>
#include <QHoverEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QProcess>
#include <QResizeEvent>
#include <QWidget>

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

    QProcess *process = new QProcess();
    process->start();

}

void EventRecorder::stop()
{
    if (!mRecording)
        return;
    mRecording = false;
    qApp->removeEventFilter(this);
    pSerializer->write(QJsonDocument(jsonArray).toJson());

    qDebug() << "Record mode was finished!";

}

QString EventRecorder::findParentObject(QObject *obj)
{
    if (!obj || !obj->parent() )
        return QString();

    QString head = this->findParentObject(obj->parent());
    if (!head.isEmpty())
        head += ".";

    head += obj->parent()->property(widgetId).toString();

    return head;
}

QJsonObject EventRecorder::logEntry(QObject *watched, QEvent *event)
{
    QJsonObject eventObject;

    eventObject.insert(className, QJsonValue::fromVariant(watched->metaObject()->className()));
    eventObject.insert(widgetName, QJsonValue::fromVariant(watched->property(widgetId).toString()));
    eventObject.insert(eventType, QJsonValue::fromVariant(event->type()));
    eventObject.insert(timestamp, QJsonValue::fromVariant(mRuntime.elapsed()));

    eventObject.insert(parents, this->findParentObject(watched));

    return eventObject;
}

void EventRecorder::logInputEvent(QObject *watched, QEvent *event)
{
    QJsonObject eventObject = this->logEntry(watched, event);

    switch (event->type()) {
        case QEvent::ActivationChange: {
            break;
        }

        case QEvent::Close:  {
        break;
        }

        case QEvent::Enter:{
            QEnterEvent *enterEvent = static_cast<QEnterEvent *>(event);
            eventObject.insert(::x, QJsonValue::fromVariant(enterEvent->localPos().x()));
            eventObject.insert(y, QJsonValue::fromVariant(enterEvent->localPos().y()));
            eventObject.insert(windowX, QJsonValue::fromVariant(enterEvent->windowPos().x()));
            eventObject.insert(windowY, QJsonValue::fromVariant(enterEvent->windowPos().y()));
            eventObject.insert(screenX, QJsonValue::fromVariant(enterEvent->screenPos().x()));
            eventObject.insert(screenY, QJsonValue::fromVariant(enterEvent->screenPos().y()));

            break;
        }

        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::FocusAboutToChange: {
            break;
        }

        case QEvent::HoverEnter:
        case QEvent::HoverLeave:
        case QEvent::HoverMove: {
            QHoverEvent *hoverEvent = static_cast<QHoverEvent *>(event);
            QVariant modifier(qApp->keyboardModifiers());
            eventObject.insert(modifiers, QJsonValue::fromVariant(modifier));
            eventObject.insert(::x, QJsonValue::fromVariant(hoverEvent->posF().x()));
            eventObject.insert(y, QJsonValue::fromVariant(hoverEvent->posF().y()));
            eventObject.insert(oldX, QJsonValue::fromVariant(hoverEvent->oldPosF().x()));
            eventObject.insert(oldY, QJsonValue::fromVariant(hoverEvent->oldPosF().y()));
            break;
        }

        case QEvent::KeyPress:
        case QEvent::KeyRelease: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            eventObject.insert(key, QJsonValue::fromVariant(keyEvent->key()));
            eventObject.insert(text, QJsonValue::fromVariant(keyEvent->text()));
            QVariant modifier(qApp->keyboardModifiers());
            eventObject.insert(modifiers, QJsonValue::fromVariant(modifier));
            break;
        }

        case QEvent::Leave:{
        break;
        }

        /*case QEvent::Move: {
            QMoveEvent *moveEvent = static_cast<QMoveEvent *>(event);

            eventObject.insert(::x, QJsonValue::fromVariant(moveEvent->pos().x()));
            eventObject.insert(y, QJsonValue::fromVariant(moveEvent->pos().y()));
            eventObject.insert(oldX, QJsonValue::fromVariant(moveEvent->oldPos().x()));
            eventObject.insert(oldY, QJsonValue::fromVariant(moveEvent->oldPos().y()));
            break;
        }*/


        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        /*case QEvent::NonClientAreaMouseMove:
        case QEvent::NonClientAreaMouseButtonPress:
        case QEvent::NonClientAreaMouseButtonRelease:*/{
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            eventObject.insert(mouseButton, QJsonValue::fromVariant(mouseEvent->button()));
            eventObject.insert(mouseButtons, QJsonValue::fromVariant(QVariant::fromValue(mouseEvent->buttons())));
            QVariant modifier(qApp->keyboardModifiers());
            eventObject.insert(modifiers, QJsonValue::fromVariant(modifier));
            eventObject.insert(::x, QJsonValue::fromVariant(mouseEvent->x()));
            eventObject.insert(y, QJsonValue::fromVariant(mouseEvent->y()));
            break;
        }

        /*case QEvent::Resize: {
            QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);

            eventObject.insert(::x, QJsonValue::fromVariant(resizeEvent->size().height()));
            eventObject.insert(y, QJsonValue::fromVariant(resizeEvent->size().width()));
            eventObject.insert(oldX, QJsonValue::fromVariant(resizeEvent->oldSize().height()));
            eventObject.insert(oldY, QJsonValue::fromVariant(resizeEvent->oldSize().width()));
            break;
        }*/

        case QEvent::UpdateRequest:{
            break;
        }


        case QEvent::WindowActivate:{
            break;
        }

        case QEvent::WindowDeactivate:{
            break;
        }

        default:
            return;
    }

    jsonArray.append(eventObject);
}

bool EventRecorder::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow")||watched->inherits("QStyle"))
        return QObject::eventFilter(watched, event);

    switch (event->type()) {
        case QEvent::ActivationChange:
        case QEvent::Close:
        case QEvent::Enter:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::FocusAboutToChange:
        case QEvent::HoverMove:
        case QEvent::HoverEnter:
        case QEvent::HoverLeave:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::Leave:
//    case QEvent::Move:
        case QEvent::MouseMove:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
//        case QEvent::NonClientAreaMouseMove:
//        case QEvent::NonClientAreaMouseButtonPress:
//        case QEvent::NonClientAreaMouseButtonRelease:
//    case QEvent::Resize:
        case QEvent::UpdateRequest:
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
            this->logInputEvent(watched, event);
            break;
        default:
            break;
    }

    return false;
}
