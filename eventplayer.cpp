#include "eventplayer.h"
#include "fileserializer.h"
#include "eventconstants.h"

#include <QApplication>
#include <QEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMouseEvent>
#include <QStringList>
#include <QTimer>
#include <QWidget>

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

    mTimerId = this->startTimer(50);
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
            mCnt++;
        }
        else break;
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
    QWidgetList widgets;
    foreach (auto widget, topLevelWidgets)
        widgets += widget->findChildren<QWidget*>();

    QString name = object.value(widgetName).toString();

    foreach (auto widget, widgets) {
        if (widget->property(widgetId).toString() == name)
            return widget;
    }

    return nullptr;
}

QEvent*EventPlayer::generateEvent(QJsonObject& object)
{
    QEvent* event = nullptr;
    QEvent::Type type = QEvent::Type(object.value(eventType).toInt(0));

    switch(object.value(eventType).toInt()){
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:{
            QPointF point(object.value("x").toInt(), object.value(y).toInt());
            Qt::MouseButton button = Qt::MouseButton(object.value(mouseButton).toInt());
            Qt::MouseButtons buttons = Qt::MouseButtons(object.value(mouseButtons).toInt());
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value(::modifiers).toInt());
            event = new QMouseEvent(type, point, button, buttons, modifiers);
            break;
        }
        case QEvent::KeyPress:
        case QEvent::KeyRelease:{
            Qt::Key key = Qt::Key(object.value(text).toInt());
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value(::modifiers).toInt());
            event = new QKeyEvent(type, key, modifiers);
            break;
        }

        default:

            break;
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
    if (mExecutiveTime.elapsed() >= timeStamp)
        return jsonObject;

    return QJsonObject();
}
