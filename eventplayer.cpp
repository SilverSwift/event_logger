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
#include <QWidget>
#include <QMessageBox>

namespace {
    static const char* PropertyName = "id_name";
}

EventPlayer::EventPlayer(AbstractSerializer* serializer, QObject *parent) :
    AbstractPlayer(serializer, parent)
{

}

EventPlayer::~EventPlayer()
{
    this->stop();
}

void EventPlayer::start()
{
    if (mPlay)
        return;

    mPlay = true;
    mPos = 0;
    mExecutiveTime.restart();

    QByteArray array = pSerializer->read();

    QJsonDocument document = QJsonDocument::fromJson(array);

    mJsonArray = document.array();

    timerId = this->startTimer(40);

}

void EventPlayer::stop()
{
    if (!mPlay){
        return;
    }
    mPlay = false;
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
    QJsonObject eventObject;

    while( !(eventObject = this->hasPendingEvents()).isEmpty() )
    {
        QObject* reciever = this->findReciever(eventObject);
        QEvent* event = this->generateEvent(eventObject);

        if (reciever && event)
            qApp->postEvent(reciever, event);

        mPos++;
    }

    if (!mPlay)
        this->killTimer(timerId);
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
    if (pathList.isEmpty() || !parent)
        return nullptr;

    auto children = parent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);

    foreach (auto child, children) {

        if (child->property(PropertyName).toString() != pathList[0])
            continue;

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

    QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();

    //problem here

    QWidget *crutchParent = new QWidget();
    foreach (auto widget, topLevelWidgets)
        widget->setParent(crutchParent);

    QWidget* widget = this->findByParent(pathList, crutchParent);

    crutchParent->deleteLater();

    return widget;
}

QEvent*EventPlayer::generateEvent(QJsonObject object)
{
    QEvent* event = nullptr;
    QEvent::Type type = QEvent::Type(object.value("eventType").toInt(0));

    switch(object.value("eventType").toInt()){
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:{
            QPointF point(object.value("x").toInt(), object.value("y").toInt());
            Qt::MouseButton button = Qt::MouseButton(object.value("mouseButton").toInt());
            event = new QMouseEvent(type, point, button, button, Qt::NoModifier);
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
            qWarning()<<"unhandled event! error!";
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

