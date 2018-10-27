#ifndef RUN_ADAPTOR_H
#define RUN_ADAPTOR_H

#include <QObject>
#include <QtDBus/QtDBus>
#include <QWidget>
#include <QMap>
#include <QVariant>
QT_BEGIN_NAMESPACE


class QString;

QT_END_NAMESPACE

/*
 * Adaptor class for interface org.tawhid.session.org
 */

class player_adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanSeek READ CanSeek)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)
    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)



    Q_CLASSINFO("D-Bus Introspection",
                ""
                "  <interface name=\"org.mpris.MediaPlayer2.Player\">\n"
                "    <property name=\"Metadata\" type=\"a{sv}\" access=\"read\" />\n"
                "    <property name=\"PlaybackStatus\" type=\"s\" access=\"read\" />\n"
                "    <property name=\"LoopStatus\" type=\"s\" access=\"readwrite\" />\n"
                "    <property name=\"Volume\" type=\"d\" access=\"readwrite\" />\n"
                "    <property name=\"Shuffle\" type=\"d\" access=\"readwrite\" />\n"
                "    <property name=\"Position\" type=\"i\" access=\"read\" />\n"
                "    <property name=\"Rate\" type=\"d\" access=\"readwrite\" />\n"
                "    <property name=\"MinimumRate\" type=\"d\" access=\"readwrite\" />\n"
                "    <property name=\"MaximumRate\" type=\"d\" access=\"readwrite\" />\n"
                "    <property name=\"CanControl\" type=\"b\" access=\"read\" />\n"
                "    <property name=\"CanPlay\" type=\"b\" access=\"read\" />\n"  //CanPlay
                "    <property name=\"CanPause\" type=\"b\" access=\"read\" />\n"
                "    <property name=\"CanSeek\" type=\"b\" access=\"read\" />\n"
                "    <property name=\"CanGoPrevious\" type=\"b\" access=\"read\" />\n"
                "    <property name=\"CanGoNext\" type=\"b\" access=\"read\" />\n"

                "    <method name=\"Previous\" />\n"
                "    <method name=\"Next\" />\n"
                "    <method name=\"Stop\" />\n"
                "    <method name=\"Play\" />\n"
                "    <method name=\"Pause\" />\n"
                "    <method name=\"PlayPause\" />\n"
                "    <method name=\"Seek\">\n"             // Seek
                "      <arg name=\"Offset\" type=\"i\" direction=\"in\" />\n"
                "    </method>"
                "    <method name=\"OpenUri\">\n"
                "      <arg type=\"s\" direction=\"in\" />\n"
                "    </method>\n"
                "    <method name=\"SetPosition\">\n"
                "      <arg type=\"o\" direction=\"in\" />\n"
                "      <arg type=\"x\" direction=\"in\" />\n"
                "    </method>\n"
                "<signal name=\"Seeked\">\n"

                "    </signal>"
                "  </interface>\n"
                "")
public:
    player_adaptor(QObject *parent);
    virtual ~player_adaptor();

Q_SIGNALS: // SIGNALS
    void Seeked(int);
public Q_SLOTS: // METHODS

    void Play();
    void Pause();
    void Stop();
    void Next();
    void Previous();
    void Seek(int Offset);
    void PlayPause();
    bool CanPlay();
    bool CanPause();
    bool CanSeek();
    bool CanGoPrevious();
    bool CanGoNext();
    QString PlaybackStatus();
    QVariantMap Metadata();
};


#endif
