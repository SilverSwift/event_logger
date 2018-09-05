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
    void inspect();
    QJsonObject logEntry(QObject *watched, QEvent *event);
    void logInputEvent(QObject *watched, QEvent *event);
    void nameWidget(QWidget* widget);

private:
    QJsonArray jsonArray;
    int mCnt = 0;
};

#endif // EVENTRECORDER_H
