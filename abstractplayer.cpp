#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer(AbstractSerializer *serializer, QObject *parent) :
     QObject(parent)
   , pSerializer(serializer)
{

}

AbstractPlayer::~AbstractPlayer()
{

}
