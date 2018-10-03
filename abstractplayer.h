#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QObject>
#include <QTime>

class AbstractSerializer;
class QThread;

class AbstractPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPlayer(AbstractSerializer *serializer,
                              QObject *parent = nullptr);

    virtual ~AbstractPlayer();

public slots:
    void start();
    void stop();

signals:
    void tryStart(QPrivateSignal);
    void tryStop(QPrivateSignal);

protected slots:
    virtual void onTryStart() = 0;
    virtual void onTryStop() = 0;

protected:
    AbstractSerializer *pSerializer = nullptr;
    QThread* pThread = nullptr;
    bool mPlay = false;

};
#endif // ABSTRACTPLAYER_H
