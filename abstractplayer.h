#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QObject>

class AbstractPlayer : public QObject
{
    Q_OBJECT
public:
    virtual void start() = 0;
    virtual void stop() = 0;

};
#endif // ABSTRACTPLAYER_H
