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
    QWidget* target = this->findByPropertyName("QMenuBar2", "MainWidget1");

    if (target){
        qDebug()<<"one";
        QTest::mouseClick(target,
                          Qt::LeftButton,
                          Qt::NoModifier,
                          QPoint(0,0),
                          10000);
        qDebug()<<"two";
    }

}

EventPlayer::~EventPlayer()
{
    this->stop();
}

void EventPlayer::start()
{
    if (mPlay){
        return;
    }
    mPlay = true;
    mPos = 0;
    QByteArray array = pSerializer->read();

    QJsonDocument document = QJsonDocument::fromJson(array);

    if(document.isArray()){
        jsonArray = document.array();
        QTime execTime;
        execTime.start();
        qDebug()<<"started"<<execTime.elapsed();
        this->handleEventArray();
        qDebug()<<"finished"<<execTime.elapsed();
    }

    mPlay = false;
}

void EventPlayer::stop()
{
    if (!mPlay){
        return;
    }
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
            QJsonObject eventObject = v.toObject();
            qDebug()<< eventObject.value("objectName").toString();
            generateEvent(eventObject, eventObject.value("timestamp").toInt() - preTime);
            preTime = eventObject.value("timestamp").toInt();
        }
        else{
            break;
        }
    }
}

void EventPlayer::timerEvent(QTimerEvent*)
{
    if (mPos < 0 || mPos >= jsonArray.size())
        return;

    QJsonObject eventObject = jsonArray.at(mPos).toObject();
    if (eventObject.isEmpty())
        return;
    this->generateEvent(eventObject);
}

void EventPlayer::generateEvent(QJsonObject obj, int delay)
{

    switch(obj.value("eventType").toInt()){
        case QEvent::MouseButtonPress:{
            QWidget* target = this->findByPropertyName(obj.value("objectName").toString(), obj.value("parent").toString());
            qDebug()<<"click"<<obj.value("objectName").toString();
            if (!target){
                qDebug()<<"i will always check my pointers!";
                return;
            }

            QTest::mouseClick(target,
                              Qt::LeftButton,
                              Qt::NoModifier,
                              QPoint(obj.value("x").toInt(),
                                     obj.value("y").toInt()),
                              delay);

            break;
        }
        case QEvent::KeyPress:{
            QWidget* target = this->findByPropertyName(obj.value("objectName").toString(), obj.value("parent").toString());
            if (!target){
                qDebug()<<"i will always check my pointers!";
                return;
            }

            QTest::keyClick(target, Qt::Key(obj.value("text").toInt()), Qt::KeyboardModifiers(obj.value("modifiers").toInt()), delay);
        }
            break;

        default: qWarning()<<"unhandled event! error!";
            break;
    }
}

QWidget* EventPlayer::findByPropertyName(QString name, QString parents)
{
    QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();
    QWidget* widget = nullptr;
    qDebug()<<"i will never use uninitialized pointers!";

    if(parents.isEmpty() ){
        foreach (QWidget *parentWidget, QApplication::topLevelWidgets()){
            if(parentWidget->property(PropertyName) == name)
                return parentWidget;
        }
    }
    else{

        QStringList parentList = parents.split('.');
        parentList.append(name);
        for(int i = 0; i < parentList.size(); i++)
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

