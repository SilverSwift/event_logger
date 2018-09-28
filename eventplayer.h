#ifndef EVENTPLAYER_H
#define EVENTPLAYER_H

#include "abstractplayer.h"
#include <QJsonArray>
#include <QJsonObject>

class EventPlayer : public AbstractPlayer
{
    Q_OBJECT
public:
    explicit EventPlayer(AbstractSerializer* serializer,
                           QObject *parent = nullptr);
    virtual ~EventPlayer() override;
    void generateEvent(QJsonObject obj, int delay = -1);

public slots:
    void start() override;
    void stop() override;

protected:
    void handleEventArray();
    virtual void timerEvent(QTimerEvent *event) override;
    QWidget* findByPropertyName(QString name, QString parents);

private:
    QJsonArray jsonArray;
    int mPos = 0;
};

#endif // EVENTPLAYER_H
