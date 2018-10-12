#include "abstractplayer.h"
#include <QThread>

#include <QDebug>

AbstractPlayer::AbstractPlayer(AbstractSerializer *serializer, QObject *parent) :
     QObject(parent)
   , pSerializer(serializer)
   , pThread(new QThread(this))
{
    connect(this, &AbstractPlayer::tryStart, this, &AbstractPlayer::onTryStart, Qt::QueuedConnection);
    connect(this, &AbstractPlayer::tryStop, this, &AbstractPlayer::onTryStop, Qt::QueuedConnection);
    this->moveToThread(pThread);
    pThread->start();
    qDebug() << "constr AbstractPlayer";
}

AbstractPlayer::~AbstractPlayer()
{
    pThread->quit();
    pThread->wait();
    this->stop();
}

void AbstractPlayer::start()
{
    qDebug() << "emit start";
    emit tryStart(QPrivateSignal());
}

void AbstractPlayer::stop()
{
    qDebug() << "emit stop";
    emit tryStop(QPrivateSignal());
}
