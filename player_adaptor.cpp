#include "player_adaptor.h"


player_adaptor::player_adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);

}

player_adaptor::~player_adaptor()
{
    // destructor

}


void player_adaptor::Seek(int Offset)
  {
     QMetaObject::invokeMethod(parent(), "seek", Q_ARG(int, Offset));

  }
void player_adaptor::Play()
{   QMetaObject::invokeMethod(parent(), "play");}

void player_adaptor::Pause()
{  QMetaObject::invokeMethod(parent(), "pause"); }

void player_adaptor::Stop()
{  QMetaObject::invokeMethod(parent(), "stop"); }

void player_adaptor::PlayPause()
{  QMetaObject::invokeMethod(parent(), "playPause"); }

void player_adaptor::Next()
{ QMetaObject::invokeMethod(parent(), "next"); }

void player_adaptor::Previous()
{  QMetaObject::invokeMethod(parent(), "previous"); }

bool player_adaptor::CanPlay()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canPlay", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal)  );

    return  retVal;
}

bool player_adaptor::CanPause()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canPause", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
}

bool player_adaptor::CanSeek()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canSeek", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
}

bool player_adaptor::CanGoPrevious()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canGoPrevious", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
}

bool player_adaptor::CanGoNext()
{

    bool retVal;
    QMetaObject::invokeMethod(parent(), "canGoNext", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
}

QString player_adaptor::PlaybackStatus()
{
    QString retVal;
    QMetaObject::invokeMethod(parent(), "playbackStatus", Qt::DirectConnection,
                              Q_RETURN_ARG(QString, retVal));
    return  retVal;
}

QVariantMap  player_adaptor::Metadata()
{
    QVariantMap map;

    QMetaObject::invokeMethod(parent(), "metadata", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariantMap, map));
    return  map;
}
