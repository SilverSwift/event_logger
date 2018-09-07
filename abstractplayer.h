#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QObject>
#include <QTime>

class AbstractSerializer;

class AbstractPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPlayer(AbstractSerializer *serializer,
                              QObject *parent = nullptr);

    virtual ~AbstractPlayer();
public slots:
    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    AbstractSerializer *pSerializer = nullptr;
    bool mPlay = false;

};
#endif // ABSTRACTPLAYER_H
