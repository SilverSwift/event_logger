#ifndef ABSTRACTSERIALIZER_H
#define ABSTRACTSERIALIZER_H

#include <QObject>

class AbstractSerializer : public QObject
{
    Q_OBJECT
public:
    AbstractSerializer(QObject *parent = nullptr);
    virtual ~AbstractSerializer();

    virtual bool write(QByteArray data) = 0;
    virtual QByteArray read() = 0;

};

#endif // ABSTRACTSERIALIZER_H
