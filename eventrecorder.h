#ifndef EVENTRECORDER_H
#define EVENTRECORDER_H

#include "abstractrecorder.h"
#include <QJsonArray>

class EventRecorder : public AbstractRecorder
{
    Q_OBJECT
public:
    explicit EventRecorder(AbstractSerializer* serializer,
                           QObject *parent = nullptr);
    virtual ~EventRecorder() override;

    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void start() override;
    void stop() override;

protected:
    QString findParentObject(QObject* obj);
    QJsonObject logEntry(QObject *watched, QEvent *event);
    void logInputEvent(QObject *watched, QEvent *event);

private:
    QJsonArray jsonArray;
};

#endif // EVENTRECORDER_H
