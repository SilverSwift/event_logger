#include "eventplayer.h"
#include "fileserializer.h"
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QList>
#include <QMouseEvent>
#include <QStringList>
#include <QtTest/QtTest>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QMessageBox>

namespace {
    static const char* PropertyName = "id_name";
}

EventPlayer::EventPlayer(AbstractSerializer* serializer, QObject *parent) :
    AbstractPlayer(serializer, parent)
  , pThread(new QThread(this))
{

//    connect(this, &EventPlayer::tryStart, this, [=](){this->onTryStart();});
//    connect(this, &EventPlayer::tryStart, this, [=](){this->onTryStop();});

    connect(this, &EventPlayer::tryStart, this, &EventPlayer::onTryStart, Qt::QueuedConnection);
    connect(this, &EventPlayer::tryStop, this, &EventPlayer::onTryStop, Qt::QueuedConnection);
    connect(this, &EventPlayer::tryPost, this, &EventPlayer::onTryPost, Qt::QueuedConnection);
    this->moveToThread(pThread);
    pThread->start();
}

EventPlayer::~EventPlayer()
{
    this->stop();
}

void EventPlayer::start()
{
//    this->onTryStart();
    emit tryStart(QPrivateSignal());

}

void EventPlayer::stop()
{
//    this->onTryStop();
    emit tryStop(QPrivateSignal());
}

void EventPlayer::print(QWidget* wgt, int indent)
{
//    qDebug()<<QString("%1%2").arg('+', indent).arg(wgt->property(PropertyName).toString());
    auto children = wgt->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    foreach (auto child, children)
        this->print(child, indent + 1);
}

void EventPlayer::onTryStart()
{
    if (mPlay)
        return;

    mPlay = true;
    mPos = 0;
    mExecutiveTime.restart();

    QByteArray array = pSerializer->read();

    QJsonDocument document = QJsonDocument::fromJson(array);

    mJsonArray = document.array();

    timerId = this->startTimer(50);
}

void EventPlayer::onTryStop()
{
    if (!mPlay){
        return;
    }
    mPlay = false;
}

void EventPlayer::onTryPost()
{
    QJsonObject eventObject;

    while( !(eventObject = this->hasPendingEvents()).isEmpty() )
    {
        auto list = QApplication::topLevelWidgets();
        foreach (auto item, list)
            this->print(item);

        QObject* reciever = this->findReciever(eventObject);
        QEvent* event = this->generateEvent(eventObject);

        if (reciever && event){
//            QTimer::singleShot(10,this, [=](){qApp->sendEvent(reciever, event);});
            qApp->postEvent(reciever, event);
//            qDebug()<<mExecutiveTime.elapsed()
//                    <<reciever->property(PropertyName).toString()
//                    <<event->type();
            qApp->processEvents();
            mPos++;
        }
        else break;
    }

    if (!mPlay)
        this->killTimer(timerId);
}

//void EventPlayer::handleEventArray()
//{
//    if(mJsonArray.size() == 0){
//        return;
//    }

//    int preTime = 0;

//    foreach (const QJsonValue & v, mJsonArray){
//        if(mPlay){
//            QJsonObject eventObject = v.toObject();
//            qDebug()<< eventObject.value("objectName").toString();
//            generateEvent(eventObject, eventObject.value("timestamp").toInt() - preTime);
//            preTime = eventObject.value("timestamp").toInt();
//        }
//        else{
//            break;
//        }
//    }
//}

void EventPlayer::timerEvent(QTimerEvent*)
{
    emit tryPost(QPrivateSignal());
}

//void EventPlayer::generateEvent(QJsonObject obj, int delay)
//{
//    switch(obj.value("eventType").toInt()){
//        case QEvent::MouseButtonPress:{
//            QWidget* target = this->findByPropertyName(obj.value("objectName").toString(), obj.value("parent").toString());
//            qDebug()<<"click"<<obj.value("objectName").toString();
//            if (!target){
//                qDebug()<<"i will always check my pointers!";
//                return;
//            }

//            QTest::mouseClick(target,
//                              Qt::LeftButton,
//                              Qt::NoModifier,
//                              QPoint(obj.value("x").toInt(),
//                                     obj.value("y").toInt()),
//                              delay);

//            break;
//        }
//        case QEvent::KeyPress:{
//            QWidget* target = this->findByPropertyName(obj.value("objectName").toString(), obj.value("parent").toString());
//            if (!target){
//                qDebug()<<"i will always check my pointers!";
//                return;
//            }

//            QTest::keyClick(target, Qt::Key(obj.value("text").toInt()), Qt::KeyboardModifiers(obj.value("modifiers").toInt()), delay);
//        }
//            break;

//        default: qWarning()<<"unhandled event! error!";
//            break;
//    }
//}

//QWidget* EventPlayer::findByPropertyName(QString name, QString parents)
//{
//    QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();
//    QWidget* widget = nullptr;
//    qDebug()<<"i will never use uninitialized pointers!";

//    if(parents.isEmpty() ){
//        foreach (QWidget *parentWidget, QApplication::topLevelWidgets()){
//            if(parentWidget->property(PropertyName) == name)
//                return parentWidget;
//        }
//    }
//    else{

//        QStringList parentList = parents.split('.');
//        parentList.append(name);
//        for(int i = 0; i < parentList.size(); i++)
//        {
//            if(i==0){
//                foreach (QWidget *parentWidget, QApplication::topLevelWidgets()){
//                    if(parentWidget->property(PropertyName) == parentList.at(i)){
//                        widget = parentWidget;
//                        break;
//                        }
//                    }
//            }else{
//                foreach(QWidget *w, widget->findChildren<QWidget*>(QString(),
//                                                                             Qt::FindDirectChildrenOnly)){
//                    if(w->property(PropertyName) == parentList.at(i)){
//                        widget = w;
//                        break;
//                        }
//                }
//            }
//        }

//    }

//    return widget;
//}

QWidget* EventPlayer::findByParent(QStringList pathList, QWidget* parent)
{
//    qDebug()<<"looking for"<<pathList.join(".")<<parent;

    if (pathList.isEmpty() || !parent)
        return nullptr;

    auto children = parent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);

    foreach (auto child, children) {

        if (child->property(PropertyName).toString() != pathList[0]){
//            qDebug()<<"skipped"<<child->property(PropertyName).toString();
            continue;
        }

//        qDebug()<<"found"<<pathList[0];

        pathList.removeAt(0);

        if (pathList.isEmpty())
            return child;
        else
            return this->findByParent(pathList, child);
    }

    return nullptr;

}

QWidget* EventPlayer::findReciever(const QJsonObject object)
{
    QStringList pathList;
    pathList.append( object.value("parent").toString().split(".") );
    pathList.append( object.value("objectName").toString() );

    QString name = object.value("objectName").toString();

    QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();


    QWidgetList widgets;
    foreach (auto widget, topLevelWidgets)
        widgets += widget->findChildren<QWidget*>();

    foreach (auto widget, widgets) {
        if (widget->property(PropertyName).toString() == name)
            return widget;
    }


/*
//    qDebug()<<"looking for"<<pathList.join(".");
    foreach (auto widget, topLevelWidgets) {
//        if (!widget) continue;
        if (widget->property(PropertyName).toString() != pathList[0]) {
//            qDebug()<<"skipped"<<widget->property(PropertyName).toString();
            continue;}
//        qDebug()<<"found"<<pathList[0];
        pathList.removeAt(0);
        return this->findByParent(pathList, widget);
    }
*/
    return nullptr;
}

QEvent*EventPlayer::generateEvent(QJsonObject object)
{
    QEvent* event = nullptr;
    QEvent::Type type = QEvent::Type(object.value("eventType").toInt(0));

    switch(object.value("eventType").toInt()){
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:{
            QPointF point(object.value("x").toInt(), object.value("y").toInt());
            Qt::MouseButton button = Qt::MouseButton(object.value("mouseButton").toInt());
            Qt::MouseButtons buttons = Qt::MouseButtons(object.value("mouseButtons").toInt());
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value("modifiers").toInt());
            event = new QMouseEvent(type, point, button, buttons, modifiers);
            break;
        }
        case QEvent::KeyPress:
        case QEvent::KeyRelease:{
            Qt::Key key = Qt::Key(object.value("text").toInt());
            Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(object.value("modifiers").toInt());
            event = new QKeyEvent(type, key, modifiers);
            break;
        }

        default:
//            qWarning()<<"unhandled event! error!";
            break;
    }
    return event;
}

QJsonObject EventPlayer::hasPendingEvents()
{
    if (mPos < 0 || mPos >= mJsonArray.size()){
        mPlay = false;
        return QJsonObject();
    }

    QJsonObject jsonObject = mJsonArray.at(mPos).toObject();
    int timeStamp = jsonObject.value("timestamp").toInt();
    if (mExecutiveTime.elapsed() >= timeStamp)
        return jsonObject;

    return QJsonObject();
}

