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

MegaEventFilter::MegaEventFilter(QObject *parent) : QObject(parent)
{
    qApp->installEventFilter(this);
    this->setWidgetName();
}

bool MegaEventFilter::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    if(     (event->type() != QEvent::MouseButtonPress) &&
            (event->type() != QEvent::KeyPress) &&
            (event->type() != QEvent::Create))
        return false;

    if(event->type() == QEvent::Create){
        this->setWidgetName();
    }else{
        QJsonObject eventObject;
        eventObject.insert("className", QJsonValue::fromVariant(watched->metaObject()->className()));
        eventObject.insert("objectName", QJsonValue::fromVariant(watched->objectName()));
        eventObject.insert("eventType", QJsonValue::fromVariant(event->type()));

        eventObject.insert("parent", this->findParentObject(watched));

        switch (event->type()) {
            case QEvent::MouseButtonPress: {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                eventObject.insert("mouseButton", QJsonValue::fromVariant(mouseEvent->button()));
                eventObject.insert("x", QJsonValue::fromVariant(mouseEvent->x()));
                eventObject.insert("y", QJsonValue::fromVariant(mouseEvent->y()));

                //TO DO: modifiers
                break;
            }
            case QEvent::KeyPress: {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                eventObject.insert("text", QJsonValue::fromVariant(keyEvent->key()));
                //eventObject.insert("modifiers", QJsonValue::fromVariant(QGuiApplication::keyboardModifiers());
                break;

            }

            default: {
                qWarning("Event handler error!!! Pizdets!!!");
            }
        }

        jsonArray.append(eventObject);

    }

    qDebug()<<Q_FUNC_INFO<<watched->metaObject()->className()<<event->type();

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
        QString parentName = obj->parent()->objectName();
        if(str.length()==0){
            str = parentName;
        }
        else {
            str = parentName % QChar('.') % str;
        }
        obj = obj->parent();
    }

    return str;
}

void MegaEventFilter::setWidgetName()
{
    foreach (QWidget *parentWidget, QApplication::topLevelWidgets()) {
        this->handleWidgetName(parentWidget);

        QList<QWidget *> widgets = parentWidget->findChildren<QWidget *>();
        foreach (QWidget* w, widgets){
            this->handleWidgetName(w);
        }

     }

}

void MegaEventFilter::handleWidgetName(QWidget* w)
{
    QString name = w->objectName();
    if(name.isEmpty()){
        name = this->createWidgetName(w->metaObject()->className());
        w->setObjectName(name);
    }

    if(! objectNames.contains(name)){
        objectNames.insert(name);
        qDebug(qUtf8Printable(w->objectName()));
    }

}

QString MegaEventFilter::createWidgetName(QString str)
{
    int objectNum = 1;
    while(objectNames.contains(str + QString::number(objectNum))){
        objectNum++;
    }

    return str + QString::number(objectNum);

}
