#include "megaeventfilter.h"
#include <QEvent>
#include <QDebug>
#include <QApplication>

MegaEventFilter::MegaEventFilter(QObject *parent) : QObject(parent)
{
    qApp->installEventFilter(this);
}

bool MegaEventFilter::eventFilter(QObject* watched, QEvent* event)
{
    qDebug()<<Q_FUNC_INFO<<watched->metaObject()->className()<<event->type();
    return false;
}
