#ifndef ABSTRACTRECORDER_H
#define ABSTRACTRECORDER_H

#include <QObject>
#include <QTime>

class AbstractSerializer;

class AbstractRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AbstractRecorder(AbstractSerializer *serializer,
                              QObject *parent = nullptr);

    virtual ~AbstractRecorder();

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    AbstractSerializer *pSerializer = nullptr;
    bool mRecording = false;
    QTime mRuntime;

};

#endif // ABSTRACTRECORDER_H
