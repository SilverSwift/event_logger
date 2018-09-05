#include "fileserializer.h"

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
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    file.write(data);

    file.close();

    return true;
}
