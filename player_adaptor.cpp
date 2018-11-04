#include "player_adaptor.h"
#include <QDBusConnection>

//---------------------------------------------------------------MainAdaptor
MainAdaptor::MainAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

MainAdaptor::~MainAdaptor()
{
    // destructor
}

void MainAdaptor:: Quit()
{  QMetaObject::invokeMethod(parent()->parent()->parent()->parent(), "close"); }

void MainAdaptor:: Raise()
{  QMetaObject::invokeMethod(parent()->parent()->parent()->parent(), "showRaise"); }

//----------------------------------------------------------PlayerAdaptor
PlayerAdaptor::PlayerAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

PlayerAdaptor::~PlayerAdaptor()
{
    // destructor

}

//--------------------------- METHODS ---------------------------

void PlayerAdaptor::Seek(int Offset)
{  QMetaObject::invokeMethod(parent(), "seek", Q_ARG(int, Offset));}

void PlayerAdaptor::Play()
{  QMetaObject::invokeMethod(parent(), "play");     }

void PlayerAdaptor::Pause()
{  QMetaObject::invokeMethod(parent(), "pause");    }

void PlayerAdaptor::Stop()
{  QMetaObject::invokeMethod(parent(), "stop");     }

void PlayerAdaptor::PlayPause()
{  QMetaObject::invokeMethod(parent(), "playPause");}

void PlayerAdaptor::Next()
{  QMetaObject::invokeMethod(parent(), "next");     }

void PlayerAdaptor::Previous()
{  QMetaObject::invokeMethod(parent(), "previous"); }

//--------------------------- PROOERTIES ---------------------------

bool PlayerAdaptor::CanPlay()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canPlay", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal)  );
    return  retVal;
}

bool PlayerAdaptor::CanPause()
{
        bool retVal;
        QMetaObject::invokeMethod(parent(), "canPause", Qt::DirectConnection,
                                  Q_RETURN_ARG(bool, retVal));
        return  retVal;
  //  return true;
}

bool PlayerAdaptor::CanSeek()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canSeek", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
}

bool PlayerAdaptor::CanGoPrevious()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canGoPrevious", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
    // return  true;
}

bool PlayerAdaptor::CanGoNext()
{
    bool retVal;
    QMetaObject::invokeMethod(parent(), "canGoNext", Qt::DirectConnection,
                              Q_RETURN_ARG(bool, retVal));
    return  retVal;
}

QString PlayerAdaptor::PlaybackStatus()
{
    QString retVal;
    QMetaObject::invokeMethod(parent(), "playbackStatus", Qt::DirectConnection,
                              Q_RETURN_ARG(QString, retVal));
    return  retVal;
}

qint64 PlayerAdaptor::Position()
{
    qlonglong retVal;
    QMetaObject::invokeMethod(parent(), "position", Qt::DirectConnection,
                              Q_RETURN_ARG(qlonglong, retVal));
    return  retVal;
}

void PlayerAdaptor::setPos(qint64 p)
{
    QVariantMap changedProps;
    changedProps.insert("Position", p);
    propertiesChanged(changedProps);
}

QVariantMap  PlayerAdaptor::Metadata()
{
    QVariantMap map;
    QMetaObject::invokeMethod(parent(), "metadata", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariantMap, map));
    return  map;
}

//--------------------------- SIGNALS ---------------------------
void PlayerAdaptor::propertiesChanged(QVariantMap changedProps)
{
    QDBusMessage signal = QDBusMessage::createSignal(
                "/org/mpris/MediaPlayer2",
                "org.freedesktop.DBus.Properties",
                "PropertiesChanged");

    signal << "org.mpris.MediaPlayer2.Player";
    signal << changedProps;
    signal << QStringList();

    if (QDBusConnection::sessionBus().send(signal))
        qDebug()<<"emited"<<signal.arguments();
    else
        qDebug()<<"No Emited"<<signal.arguments();
}
