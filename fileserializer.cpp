#include "fileserializer.h"

#include <QDebug>
#include <QFile>

FileSerializer::FileSerializer(QString fileName, QObject *parent) :
    AbstractSerializer(parent)
  , mFileName(fileName)
{
}

FileSerializer::~FileSerializer()
{

}

bool FileSerializer::write(QByteArray data)
{
    QFile file (mFileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug()<<file.errorString();
        return false;
    }

    file.write(data);

    file.close();

    return true;
}

QByteArray FileSerializer::read()
{
    QFile file (mFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<< file.errorString();
        return QByteArray();
    }

    QByteArray data = file.readAll();

    file.close();

    return data;
}
