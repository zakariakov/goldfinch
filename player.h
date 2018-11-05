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

#ifndef PLAYER_H
#define PLAYER_H
#include "playercontrols.h"
#include "widgetplaylist.h"
#include "player_adaptor.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QListView>
QT_BEGIN_NAMESPACE
class QAbstractItemView;

class QMediaPlayer;
class QModelIndex;


QT_END_NAMESPACE

class PlaylistModel;

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QListView *playlist,QWidget *parent = nullptr);
    ~Player();


    void addToPlaylist(QList<QVariantMap> &files);
    void addToPlaylist( QList<QUrl> &urls);


    void playLast();


signals:

    void statuInfoChanged(QString );
    void playBackChanged(QString,int=0 );
    void iconsChanged();
    void imageChanged(QImage);
    void titleChanged(QString,QString);
    void infoChanged(QString);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void updateSong(QVariantMap,QString);
 void propertiesChanged(QString,QVariantMap,QStringList);

public slots:

    // dbus
    void play() {   mPlayer->play();   }
    void pause(){   mPlayer->pause();  }
    void stop() {   mPlayer->stop();   }
    void next() {   mPlaylist->next(); }
    void previous();
    void playPause();
    void setSeek(int seconds);
    qint64 position(){return mPlayer->position(); }
    bool canPlay() {
       if(mPlayer->isAvailable())return false;
       return true;
    }
    bool canPause() {
       if(!mPlayer->isAvailable()) return false;
       return true;
    }
    bool canGoNext();
    bool canGoPrevious();
    bool canSeek(){return  mPlayer->isSeekable();}
    QVariantMap metadata(){return mMetaDataMap;}
    QString playbackStatus(){return mPlaybackStatus;}
    //--
    void rmovePlaylistItem(QModelIndex idx);
    void moveMedia(int from,int to);
    void cleanList();
    void setPlaybackMode(int value);
    void setFile(const QString &file);
private slots:

    void metaDataChanged();
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);

    void displayErrorMessage();
    void save(const QString &name);
    void openSavedList(QString name);
    void setduration(qint64 duration);
private:

    void setTrackInfo  (const QString &info);
    void setStatusInfo (const QString &info);
    void handleCursor  (QMediaPlayer::MediaStatus status);
    void setCovertImage(QImage img, QString album);

    PlayerAdaptor  *mPlayerAdaptor;
    QMediaPlayer   *mPlayer = nullptr;
    QMediaPlaylist *mPlaylist = nullptr;
    PlayerControls *mControls;
    PlaylistModel  *mPlaylistModel = nullptr;
    QListView      *mPlaylistView ;
    QVariantMap     mMetaDataMap;
    QString         mTrackInfo;
    QString         mStatusInfo;
    QString         mPlaybackStatus;


 // FreeDesktopAdaptor *mFreeDesktopAdaptor;
};

#endif // PLAYER_H
