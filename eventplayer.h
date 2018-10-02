#ifndef EVENTPLAYER_H
#define EVENTPLAYER_H

#include "abstractplayer.h"
#include <QJsonArray>
#include <QTime>

#include <QJsonObject>

class QThread;

class EventPlayer : public AbstractPlayer
{
    Q_OBJECT
public:
    explicit EventPlayer(AbstractSerializer* serializer,
                           QObject *parent = nullptr);
    virtual ~EventPlayer() override;
//    void generateEvent(QJsonObject obj, int delay);

public slots:
    void start() override;
    void stop() override;

signals:
    void tryStart(QPrivateSignal);
    void tryStop(QPrivateSignal);
    void tryPost(QPrivateSignal);

protected:
    void print(QWidget* wgt, int indent = 1);
    void onTryStart();
    void onTryStop();
    void onTryPost();
//    void handleEventArray();
    virtual void timerEvent(QTimerEvent *event) override;
//    QWidget* findByPropertyName(QString name, QString parents);
    QWidget* findByParent(QStringList pathList, QWidget* parent);
    QWidget* findReciever(const QJsonObject object);
    QEvent* generateEvent(QJsonObject object);
    QJsonObject hasPendingEvents();

private:
    QJsonArray mJsonArray;
    int mPos = 0;
    int timerId = -1;
    QTime mExecutiveTime;

    QThread* pThread;
};

#endif // EVENTPLAYER_H
