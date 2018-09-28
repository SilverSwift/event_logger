#include "fileserializer.h"

#include <QFile>

#include <QDebug>

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
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    file.write(data);

    file.close();

    return true;
}

QByteArray FileSerializer::read()
{
    QFile file (mFileName);
    //if (!file.open(QFile::ReadOnly)){
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<< file.errorString();
        return QByteArray();
    }

    QByteArray data = file.readAll();

    file.close();

    return data;
}
