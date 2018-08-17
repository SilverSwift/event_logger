#include "megaeventfilter.h"
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtWidgets>

MegaEventFilter::MegaEventFilter(QObject *parent) : QObject(parent)
{
    qApp->installEventFilter(this);
}

bool MegaEventFilter::eventFilter(QObject* watched, QEvent* event)
{
    if(watched->inherits("QWidgetWindow"))
        return QObject::eventFilter(watched, event);

    if(     (event->type() != QEvent::MouseButtonPress) &&
            (event->type() != QEvent::KeyPress))
        return false;

    QJsonObject eventObject;
    eventObject.insert("className", QJsonValue::fromVariant(watched->metaObject()->className()));
    eventObject.insert("objectName", QJsonValue::fromVariant(watched->objectName()));
    eventObject.insert("eventType", QJsonValue::fromVariant(event->type()));

    eventObject.insert("parent", this->findParentObject(watched).value("parent"));

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
            //eventObject.insert("modifiers", QJsonValue::fromVariant(Qt::KeyboardModifiers())); //модификаторы, несколько?
            break;

        }

        default: {
            qWarning("Event handler error!!! Pizdets!!!");
        }
    }

    jsonArray.append(eventObject);

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

QJsonObject MegaEventFilter::findParentObject(QObject* obj)
{
    QJsonObject object;
    while(obj && obj->parent()){
        QJsonObject parentObject;
        parentObject.insert("className", QJsonValue::fromVariant(obj->parent()->metaObject()->className()));
        parentObject.insert("objectName", QJsonValue::fromVariant(obj->parent()->objectName()));
        object.insert("parent", parentObject);
        obj = obj->parent();
    }

    return object;
}
