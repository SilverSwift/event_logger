#ifndef FILESERIALIZER_H
#define FILESERIALIZER_H

#include "abstractserializer.h"
#include <QJsonArray>

class FileSerializer : public AbstractSerializer
{
    Q_OBJECT
public:
    explicit FileSerializer(QString fileName, QObject *parent = nullptr);
    ~FileSerializer() override;

    virtual bool write(QByteArray data) override;

private:
    QString mFileName;
};

#endif // FILESERIALIZER_H
