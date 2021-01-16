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
#include "actions.h"
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
    void setMiniPlayer(bool mini){mControls->togglePreview(mini);}

signals:

    void statuInfoChanged(QString );
    void playBackChanged(QString,int=0 );
//    void iconsChanged();
    void imageChanged(QImage);
    void titleChanged(QString,QString);
    void infoChanged(QString);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void appCloseChanged();
    void appRaiseChanged();
    void appHideChanged();
    void apptoggleHideChanged();
//    void updateSong(QVariantMap,QString);
// void propertiesChanged(QString,QVariantMap,QStringList);

public slots:

  void  setVolumeUp(){mPlayer->setVolume(mPlayer->volume()+5);}
  void  setVolumeDown(){mPlayer->setVolume(mPlayer->volume()-5);}
  void  setVolumeMuteUnmute(bool mute){mPlayer->setMuted(mute);}

    // dbus
    void appHide()  {emit appHideChanged();}
    void appQuit()  {emit appCloseChanged();}
    void showRaise(){emit appRaiseChanged();}
    void toggleHide(){emit apptoggleHideChanged();}
    void play()   {    mPlayer->play();    }
    void pause(){   mPlayer->pause();  }
    void stop()  {    mPlayer->stop();    }
    void next()  {    mPlaylist->next();   }
    void previous();
    void playPause();
    void setSeek(int seconds);
     void setSeekPlus();
      void setSeekMinus();
    void seek(qlonglong Offset);
    qint64 position(){return mPlayer->position()*1000; }
    bool canPlay()     { if(!mPlayer->isAvailable()){return false;} else {   return true;}  }
     bool canPause() { if(!mPlayer->isAvailable()) { return false; } else { return true;}    }
    bool canGoNext(){ if(mPlaylist->currentIndex()<(mPlaylist->mediaCount()-1)) { return true; }  else {return false;} }
    bool canGoPrevious(){ if(mPlaylist->currentIndex()>0) { return true; } else { return false;  }  }
    bool canSeek(){return  mPlayer->isSeekable();}
    QVariantMap metadata(){return mMetaDataMap;}
    QString playbackStatus(){return mPlaybackStatus;}
    //--
    void rmovePlaylistItem();
    void moveMedia(int from,int to);
    void cleanList();
    void setPlaybackMode(int value);
    void setFile(const QString &file);
    void saveList();
private slots:

    void metaDataChanged();
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);

    void displayErrorMessage();
void setseekold();
    void openSavedList();
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
qint64 mOldPos=0;

 // FreeDesktopAdaptor *mFreeDesktopAdaptor;
};

#endif // PLAYER_H
