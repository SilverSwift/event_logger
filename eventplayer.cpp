#include "eventplayer.h"
#include "fileserializer.h"
#include "eventconstants.h"

#include <QApplication>
#include <QEvent>
#include <QFocusEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMouseEvent>
#include <QSize>
#include <QStringList>
#include <QTimer>
#include <QWindowStateChangeEvent>
#include <QWidget>

#include <QDebug>

EventPlayer::EventPlayer(AbstractSerializer* serializer, QObject *parent) :
    AbstractPlayer(serializer, parent) 
{
    connect(this, &EventPlayer::tryPost,
            this, &EventPlayer::onTryPost, Qt::QueuedConnection);

}

EventPlayer::~EventPlayer()
{

}

void EventPlayer::onTryStart()
{
    if (mPlay)
        return;

    mPlay = true;
    mCnt = 0;
    mExecutiveTime.restart();

    QByteArray array = pSerializer->read();

    QJsonDocument document = QJsonDocument::fromJson(array);

    mJsonArray = document.array();

    mTimerId = this->startTimer(10);
}

void EventPlayer::onTryStop()
{
    if (!mPlay)
        return;

    mPlay = false;
}

void EventPlayer::onTryPost()
{

    QJsonObject eventObject;
    while( !(eventObject = this->hasPendingEvents()).isEmpty() )
    {
        QObject* reciever = this->findReciever(eventObject);
        QEvent* event = this->generateEvent(eventObject);

        if (reciever && event){
            qApp->postEvent(reciever, event);
            qDebug() << reciever->property("widgetId") << event->type();
            mCnt++;
        }else{
            qDebug() << "Reciever or event not found!";
            this->onTryStop();
            break;

        }
    }
    if (!mPlay)
        this->killTimer(mTimerId);
}


void EventPlayer::timerEvent(QTimerEvent*)
{
    emit tryPost(QPrivateSignal());
}

QWidget* EventPlayer::findReciever(const QJsonObject& object)
{
    QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();

    QString name = object.value(widgetName).toString();

    if(object.value(parents).toString().isEmpty()){
        foreach (auto widget, topLevelWidgets) {
            if (widget->property(widgetId).toString() == name)
                return widget;
        }
    } else {
        QWidgetList widgets;
        foreach (auto widget, topLevelWidgets)
            widgets += widget->findChildren<QWidget*>();

        foreach (auto widget, widgets) {
            if (widget->property(widgetId).toString() == name)
                return widget;
        }
    }

    return nullptr;
}

QEvent*EventPlayer::generateEvent(QJsonObject& object)
{
    QEvent* event = nullptr;
    QEvent::Type type = QEvent::Type(object.value(eventType).toInt(0));

    switch(object.value(eventType).toInt()){
        case QEvent::ActivationChange: {
            event = new QEvent(type);
            break;
        }

        case QEvent::Enter:{

            QPointF localPoint(object.value(::x).toInt(), object.value(y).toInt());
            QPointF windowPoint(object.value(windowX).toInt(), object.value(windowY).toInt());
            QPointF screenPoint(object.value(screenX).toInt(), object.value(screenY).toInt());
            event = new QEnterEvent(localPoint, windowPoint, screenPoint);
            break;
        }

        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::FocusAboutToChange: {
            event = new QFocusEvent(type, Qt::FocusReason::MouseFocusReason);
            break;
        }

        case QEvent::HoverEnter:
        case QEvent::HoverLeave:
        case QEvent::HoverMove: {
            QPointF point(object.value(::x).toInt(), object.value(y).toInt());
            QPointF oldPoint(object.value(oldX).toInt(), object.value(oldY).toInt());
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value(::modifiers).toInt());
            event = new QHoverEvent(type, point, oldPoint, modifiers);
            break;
        }

        case QEvent::KeyPress:
        case QEvent::KeyRelease:{
            Qt::Key key = Qt::Key(object.value(::key).toInt());
            QString keyText = object.value(text).toString();
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value(::modifiers).toInt());
            event = new QKeyEvent(type, key, modifiers, keyText);
            break;
        }

        case QEvent::LayoutRequest:
        case QEvent::Leave: {
            event = new QEvent(type);
            break;
        }

        case QEvent::Move: {
            QPoint point(object.value(::x).toInt(), object.value(y).toInt());
            QPoint oldPoint(object.value(oldX).toInt(), object.value(oldY).toInt());

            event = new QMoveEvent(point, oldPoint);
            break;
        }

        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::NonClientAreaMouseMove:
        case QEvent::NonClientAreaMouseButtonPress:
        case QEvent::NonClientAreaMouseButtonRelease:{
            QPointF point(object.value("x").toInt(), object.value(y).toInt());
            Qt::MouseButton button = Qt::MouseButton(object.value(mouseButton).toInt());
            Qt::MouseButtons buttons = Qt::MouseButtons(object.value(mouseButtons).toInt());
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value(::modifiers).toInt());
            event = new QMouseEvent(type, point, button, buttons, modifiers);
            break;
        }

        case QEvent::Resize: {
            QSize size(object.value(::x).toInt(), object.value(y).toInt());
            QSize oldSize(object.value(oldX).toInt(), object.value(oldY).toInt());
            event = new QResizeEvent(size, oldSize);
            break;
        }

        case QEvent::StyleAnimationUpdate:
        case QEvent::UpdateRequest:
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
        {
            event = new QEvent(type);
            break;
        }

        default: {
            qDebug() << "error"<<object.value(timestamp) << object.value(eventType) << object.value(eventType).toInt();
            break;
        }
    }
    return event;
}

QJsonObject EventPlayer::hasPendingEvents()
{
    if (mCnt < 0 || mCnt >= mJsonArray.size()){
        mPlay = false;
        return QJsonObject();
    }

    QJsonObject jsonObject = mJsonArray.at(mCnt).toObject();
    int timeStamp = jsonObject.value(timestamp).toInt();

    if (mExecutiveTime.elapsed() >= timeStamp){
        qDebug() << mExecutiveTime.elapsed() << mCnt;
        return jsonObject;
    }

    return QJsonObject();
}
