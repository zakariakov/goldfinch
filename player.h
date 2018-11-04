/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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

    bool isPlayerAvailable() const;

    void addToPlaylist(QList<QVariantMap> &urls);
    void addToPlaylist( QList<QUrl> &urls);


    void playLast();


signals:

    void statuInfoChanged(QString );
    void playBackChanged(QString,int=0 );
    void iconsChanged();
    void imageChanged(QImage);
    void titleChanged(QString);
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
       if(!mPlayer->isAvailable())return false;
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
    PlayerControls *controls;
    PlaylistModel  *mPlaylistModel = nullptr;
    QListView      *mPlaylistView ;
    QVariantMap     mMetaDataMap;
    QString         mTrackInfo;
    QString         mStatusInfo;
    QString         mPlaybackStatus;


 // FreeDesktopAdaptor *mFreeDesktopAdaptor;
};

#endif // PLAYER_H
