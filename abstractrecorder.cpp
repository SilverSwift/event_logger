#include "abstractrecorder.h"

AbstractRecorder::AbstractRecorder(AbstractSerializer *serializer, QObject *parent) :
     QObject(parent)
   , pSerializer(serializer)
{

}

AbstractRecorder::~AbstractRecorder()
{

}
