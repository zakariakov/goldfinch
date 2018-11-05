/***************************************************************************
 *      Project created by QtCreator 2018-06-01T17:15:24                   *
 *                                                                         *
 *    goldfinch Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef RUN_ADAPTOR_H
#define RUN_ADAPTOR_H

#include <QObject>
#include <QtDBus/QtDBus>
#include <QWidget>
#include <QMap>
#include <QVariant>
QT_BEGIN_NAMESPACE

#include <QApplication>
class QString;

QT_END_NAMESPACE
/*
 * Adaptor class for interface org.mpris.MediaPlayer2
 */

class MainAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    Q_PROPERTY(bool CanQuit READ CanQuit)
    Q_PROPERTY(bool CanRaise READ CanRaise)
    Q_PROPERTY(QString Identity READ Identity)
    Q_PROPERTY(QString DesktopEntry READ DesktopEntry)

    Q_CLASSINFO("D-Bus Introspection",
                "  <interface name=\"org.mpris.MediaPlayer2\">\n"
                "    <property name=\"Identity\" type=\"s\" access=\"read\" />\n"
                "    <property name=\"DesktopEntry\" type=\"s\" access=\"read\" />\n"
                "    <property name=\"CanQuit\" type=\"b\" access=\"read\" />\n"
                "    <property name=\"CanRaise\" type=\"b\" access=\"read\" />\n"
                "    <method name=\"Quit\" />\n"
                "    <method name=\"Raise\" />\n"
               "        <arg name=\"url\" type=\"s\" direction=\"in\"/>"
                "      <arg type=\"s\" />\n"
                "    </method>\n"
                "  </interface>\n"
                )
public:
    MainAdaptor(QObject *parent);
    virtual ~MainAdaptor();

public Q_SLOTS: // METHODS

    void Quit();
    void Raise();
    bool CanQuit() {return true;}
    bool CanRaise(){return true;}

    QString DesktopEntry(){return QApplication::applicationName();}
    QString Identity()    {return QApplication::applicationName();}

    void SetUrl(const QString &url);
};


/*
 * Adaptor class for interface org.mpris.MediaPlayer2.Player
 */

class PlayerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanSeek READ CanSeek)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)
    Q_PROPERTY(bool CanControl READ CanControl)
    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)
    Q_PROPERTY(qint64 Position READ Position)

    Q_CLASSINFO("D-Bus Introspection",

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
                "    <signal name=\"Seeked\">\n"
                "    </signal>"
                "  </interface>\n"
                "")
public:
    PlayerAdaptor(QObject *parent);
    virtual ~PlayerAdaptor();

Q_SIGNALS: // SIGNALS
    void Seeked(qint64);

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
    bool CanControl(){return true;}
    QString PlaybackStatus();
    qint64 Position();
    void setPos(qint64 p);
    QVariantMap Metadata();

    void propertiesChanged(QVariantMap changedProps);

private slots:

    //   FreeDesktopAdaptor* mFreeDesktopAdaptor;
};


#endif
