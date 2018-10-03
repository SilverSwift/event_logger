#ifndef EVENTPLAYER_H
#define EVENTPLAYER_H

#include "abstractplayer.h"
#include <QJsonArray>

class EventPlayer : public AbstractPlayer
{
    Q_OBJECT
public:
    explicit EventPlayer(AbstractSerializer* serializer,
                           QObject *parent = nullptr);
    virtual ~EventPlayer() override;

signals:
    void tryPost(QPrivateSignal);

protected:
    QWidget* findReciever(const QJsonObject& object);
    QEvent* generateEvent(QJsonObject& object);
    QJsonObject hasPendingEvents();
    void onTryPost() ;

    virtual void onTryStart() override;
    virtual void onTryStop() override;
    virtual void timerEvent(QTimerEvent *event) override;

private:
    QJsonArray mJsonArray;
    int mCnt = 0;
    QTime mExecutiveTime;
    int mTimerId = -1;
};

#endif // EVENTPLAYER_H
