#include "abstractplayer.h"
#include <QThread>

AbstractPlayer::AbstractPlayer(AbstractSerializer *serializer, QObject *parent) :
     QObject(parent)
   , pSerializer(serializer)
   , pThread(new QThread(this))
{
    connect(this, &AbstractPlayer::tryStart, this, &AbstractPlayer::onTryStart, Qt::QueuedConnection);
    connect(this, &AbstractPlayer::tryStop, this, &AbstractPlayer::onTryStop, Qt::QueuedConnection);
    this->moveToThread(pThread);
    pThread->start();
}

AbstractPlayer::~AbstractPlayer()
{
    this->stop();
}

void AbstractPlayer::start()
{
    emit tryStart(QPrivateSignal());
}

void AbstractPlayer::stop()
{
    emit tryStop(QPrivateSignal());
}
