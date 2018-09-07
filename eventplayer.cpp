#include "eventplayer.h"
#include "fileserializer.h"
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QList>
#include <QStringList>
#include <QTimer>
#include <QWidget>

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
    jsonArray = QJsonDocument::fromBinaryData(pSerializer->read()).array();
    this->handleEventArray();
    mPlay = false;
}

void EventPlayer::stop()
{
    if (!mPlay)
        return;
    mPlay = false;
}

void EventPlayer::handleEventArray()
{
    if(jsonArray.size() == 0){
        return;
    }

    int preTime = 0;

    foreach (const QJsonValue & v, jsonArray){
        if(mPlay){
            this->inspect();
            QJsonObject eventObject = v.toObject();
            QTimer::singleShot(eventObject.value("timestamp").toInt() - preTime, this, SLOT(generateEvent(eventObject)));
            preTime = eventObject.value("timestamp").toInt();
        }
        else{
            break;
        }
    }
}

void EventPlayer::generateEvent(QJsonObject obj)
{
    switch(obj.value("eventType").toInt()){
    case QEvent::MouseButtonPress:{
        this->mousePress(this->findByPropertyName(obj.value("objectName").toString(), obj.value("parent").toString()),
                         obj.value("x").toInt(),
                         obj.value("y").toInt(),
                         Qt::MouseButton(obj.value("mouseButton").toInt()));
        break;
    }
    case QEvent::KeyPress:{
        this->keyPress(this->findByPropertyName(obj.value("objectName").toString(), obj.value("parent").toString()),
                       obj.value("mouseButton").toInt(),
                       Qt::KeyboardModifiers(obj.value("mouseButton").toInt()));
        break;
    }
    default: qWarning()<<"unhandled event! error!";
        break;
    }
}

QWidget* EventPlayer::findByPropertyName(QString name, QString parents)
{
    QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();
    QWidget* widget;

    if(parents.isEmpty() ){
        foreach (QWidget *parentWidget, QApplication::topLevelWidgets()){
            if(parentWidget->property(PropertyName) == name)
                return parentWidget;
        }
    }
    else{

        QStringList parentList = parents.split('.');
        parentList.append(name);
        for(int i = 0; i < parentList.size(); ++i)
        {
            if(i==0){
                foreach (QWidget *parentWidget, QApplication::topLevelWidgets()){
                    if(parentWidget->property(PropertyName) == parentList.at(i)){
                        widget = parentWidget;
                        break;
                        }
                    }
            }else{
                foreach(QWidget *w, widget->findChildren<QWidget*>(QString(),
                                                                             Qt::FindDirectChildrenOnly)){
                    if(w->property(PropertyName) == parentList.at(i)){
                        widget = w;
                        break;
                        }
                }
            }
        }

    }

    return widget;
}

void EventPlayer::inspect()
{
    foreach (QWidget *parentWidget, QApplication::topLevelWidgets())
        this->nameWidget(parentWidget);
}

void EventPlayer::nameWidget(QWidget *widget)
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

void EventPlayer::mousePress(QWidget* wgt,
                int x,
                int y,
                Qt::MouseButton bt,
                Qt::MouseButtons bts)
{
    if (wgt) {
        QMouseEvent* pePress = new QMouseEvent(QEvent::MouseButtonPress, QPoint(x, y), bt, bts, Qt::NoModifier);
        QApplication::postEvent(wgt, pePress);
    }
}

void EventPlayer::keyPress(QWidget* wgt, int key, Qt::KeyboardModifiers modifiers)
{
    if(wgt) {
        QKeyEvent keyPress(QEvent::KeyPress, key, modifiers);
        QApplication::postEvent(wgt, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyPress, key, modifiers);
        QApplication::postEvent(wgt, &keyRelease);
    }
}
