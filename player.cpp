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

#include "player.h"

#include "tumb.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "player_adaptor.h"
#include <QMediaPlaylist>

#include <QMediaMetaData>
#include <QtWidgets>
#include <QFontMetrics>
#include <QSettings>
Player::Player(QListView *playlist, QWidget *parent)
    : QWidget(parent),m_playlistView(playlist)
{

    m_player = new QMediaPlayer(this);

    qInfo() << "Supported audio roles:";
    for (QAudio::Role role : m_player->supportedAudioRoles())
        qInfo() << "    " << role;
    foreach (QString s, m_player->supportedMimeTypes()){
        qInfo() << "    " << s;
    }
    // owned by PlaylistModel
    m_playlist = new QMediaPlaylist();
    m_player->setPlaylist(m_playlist);

player_adaptor *mPlayer_adaptor=new player_adaptor(this);

    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &Player::metaDataChanged);
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &Player::statusChanged);
    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &Player::bufferingProgress);
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &Player::displayErrorMessage);


    m_playlistModel = new PlaylistModel(this);
    m_playlistModel->setPlaylist(m_playlist);

    m_playlistView->setModel(m_playlistModel);
    m_playlistView->setCurrentIndex(m_playlistModel->index(m_playlist->currentIndex(), 0));

    connect(m_playlistView, &QAbstractItemView::activated, this, &Player::jump);


    PlayerControls *controls = new PlayerControls(this);
    controls->setState(m_player->state());
    controls->setVolume(m_player->volume());

    connect(m_player, &QMediaPlayer::durationChanged, controls, &PlayerControls::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, controls, &PlayerControls::positionChanged);
    connect(m_player, &QMediaPlayer::positionChanged, mPlayer_adaptor, &player_adaptor::Seeked);

    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
    connect(controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
    connect(controls, &PlayerControls::previous, this, &Player::previous);
    connect(controls, &PlayerControls::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
    connect(controls, &PlayerControls::seek, this, &Player::seek);

    connect(this, &Player::iconsChanged, controls, &PlayerControls::setupIcons);
 //   connect(this, &Player::canPlay, controls, &PlayerControls::setPlayEnabled);
  //  connect(this, &Player::canNext, controls, &PlayerControls::setNextEnabled);
  //  connect(this, &Player::canPrev, controls, &PlayerControls::setPrevEnabled);

    connect(m_player, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(m_player, &QMediaPlayer::stateChanged, this, &Player::stateChanged);
    connect(m_player, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
   connect(m_player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
  //   connect(m_player, &QMediaPlayer::metaDataChanged,this, &Player::metaDataChanged);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    hLayout->addSpacing(3);
    hLayout->addWidget(controls);
    hLayout->addSpacing(3);


    setLayout(hLayout);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
        m_playlistView->setEnabled(false);
    }

    openSavedList("playlist");
  //  controls->setEnabled(false);
    metaDataChanged();

}

Player::~Player()
{
    save("playlist");
}
//-----------------------------------------------------------------
void Player::rmovePlaylistItem(QModelIndex idx)
{
    int row=idx.row();
    m_playlist->removeMedia(row);
    //  m_playlistModel->removeRow(idx.row());
    m_playlistView->setCurrentIndex(m_playlistModel->index(row,0));
}

void Player::moveMedia(int from,int to)
{

    if(m_playlist->moveMedia(from,to))
        m_playlistView->setCurrentIndex(m_playlistModel->index(to,0));
}

void Player::cleanList()
{
    //  qDebug()<<"cleanup";
    m_playlist->clear();
    m_playlistModel->clear();
}

//-----------------------------------------------------------
bool Player::isPlayerAvailable() const
{
    return m_player->isAvailable();
}

void Player::addToPlaylist( QList<QVariantMap> &urls)
{
    QList<QMap<QString,QUrl>> list;
    QMap<QString,QUrl> map;

    int count=m_playlist->mediaCount();

    /*
    for (auto &url: urls) {

        m_playlist->addMedia(url);
    }
*/
    for (int i = 0; i < urls.count(); ++i) {
        QMap<QString,QVariant> map=urls.at(i);

        QUrl url;
        QString str=map.value("Url").toString();
        if(str.startsWith("https:")){
          url.setUrl(str);
         m_playlist->addMedia(url);
        }else{

               url.setUrl(QUrl::fromLocalFile(str).toString());
              m_playlist->addMedia(url);
        }

        QString tit=map.value("Title").toString();
        QModelIndex idx=m_playlistModel->index(count+i,0);
        m_playlistModel->setTitle(idx,url,tit);

    }

}
void Player::addToPlaylist( QList<QUrl> &urls)
{
    for (auto &url: urls) {
        m_playlist->addMedia(url);
    }
}


void Player::metaDataChanged()
{
    if (m_player->isMetaDataAvailable()) {

        //---------------------------------------
        QFileInfo fi=    m_playlist->currentMedia().canonicalUrl().toLocalFile();
        QString   murl=  m_playlist->currentMedia().canonicalUrl().toString();
        QString   title= m_player->metaData(QMediaMetaData::Title).toString();
        QString   album= m_player->metaData(QMediaMetaData::AlbumTitle).toString();
        QString   artist=m_player->metaData(QMediaMetaData::ContributingArtist).toString();
        QString   albumArtist=m_player->metaData(QMediaMetaData::AlbumArtist).toString();
        QString   genre= m_player->metaData(QMediaMetaData::Genre).toString();
        qlonglong lenght=QVariant(m_player->duration()).toLongLong();
        QImage    img=m_player->metaData(QMediaMetaData::CoverArtImage).value<QImage>();

        QString duration=QTime::fromMSecsSinceStartOfDay(QVariant(lenght).toInt()).toString();

        title=title.isEmpty() ? fi.fileName() : title;
        artist=artist.isEmpty()? albumArtist: artist;

        //---------------------------------------
        mMetaDataMap.clear();
        mMetaDataMap.insert("xesam:title",title);
        mMetaDataMap.insert("xesam:album",album);
        mMetaDataMap.insert("xesam:url",murl);
        mMetaDataMap.insert("xesam:artist",artist);
        mMetaDataMap.insert("mpris:length",lenght);

        //---------------------------------------
        QString strTit=title.isEmpty() ? fi.fileName() : title;
        QString strInf=QString("%1 - %2").arg(artist).arg(album);

        emit titleChanged(title);
        emit infoChanged(strInf);

        //---------------------------------------
        //        QString artSmall= m_player->metaData(QMediaMetaData::CoverArtUrlSmall).toString();
        //        QString artLarge=m_player->metaData(QMediaMetaData::CoverArtUrlLarge).toString();

        //--------------------------------------
        int in=  m_playlist->currentIndex();
        QModelIndex idx=m_playlistModel->index(in,0);
        QUrl url= m_playlist->currentMedia().canonicalUrl();
        m_playlistModel->setTitle(idx,url,title);
        setTrackInfo(title);

        //---------------------------------------
        QVariantMap map;
        map["title"]=title;
        map["artist"]=artist;
        map["album"]=album;
        map["genre"]=genre;
        map["path"]=fi.filePath();  //TODO FIX URL
        map["duration"]=duration;
        emit updateSong(map,fi.filePath());
        //---------------------------------------
        setCovertImage(img,album);


    }

}

void Player::setCovertImage(QImage img,QString album)
{
//    qDebug()<<"image size:"<<img.width();
//    qDebug()<<"artSmall  :"<<artSmall;
//    qDebug()<<"artLarge  :"<<artLarge;

    if(!img.isNull()){
        //   mLabelimg->setPixmap(QPixmap::fromImage(img));
        emit imageChanged(img);
        return;
    }

    QFileInfo fi=m_playlist->currentMedia().canonicalUrl().toLocalFile();
    //  qDebug()<<"QFileInfo:"<<fi.filePath();
    QString path=fi.absolutePath();
    //  TODO: fix title album
    QIcon icon=Tumb::iconAlbum((album.isEmpty() ? fi.absoluteDir().dirName():album),path);
    QString imgPath= Tumb::imageAlbumPath(
                (album.isEmpty() ? fi.absoluteDir().dirName():album),path);
    img.load(imgPath);
    emit imageChanged(img);

}

void Player::previous()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (m_player->position() <= 5000)
        m_playlist->previous();
    else
        m_player->setPosition(0);
}
void Player::playLast()
{

    QModelIndex idx=m_playlistModel->index( m_playlist->mediaCount()-1,0);
    if (idx.isValid()) {
        m_playlist->setCurrentIndex(idx.row());
        m_player->play();
    }

}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        m_playlist->setCurrentIndex(index.row());
        m_player->play();
    }
}

void Player::playlistPositionChanged(int currentItem)
{
    m_playlistView->setCurrentIndex(m_playlistModel->index(currentItem, 0));
//    emit canPlay(m_playlist->mediaCount()>0);
//    emit canNext(m_playlist->currentIndex()<(m_playlist->mediaCount()-1)) ;
//    emit canPrev(m_playlist->currentIndex()>0);

}

void Player::seek(int seconds)
{
    m_player->setPosition(seconds * 1000);
}

void Player::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:

    case QMediaPlayer::LoadedMedia:
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }


}


void Player::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
            status == QMediaPlayer::BufferingMedia ||
            status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void Player::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}


void Player::setStatusInfo(const QString &info)
{
    m_statusInfo = info;
    if (!m_statusInfo.isEmpty())
        //    setStatusTip()
        emit  playBackChanged(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
    else{
        emit   playBackChanged(m_trackInfo);

        stateChanged(m_player->state());
    }

     //emit playBackChanged
}

void Player::displayErrorMessage()
{
    emit  playBackChanged(m_player->errorString());
}

void Player::playPause()
{
    switch (m_player->state()) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
         play();
        break;
    case QMediaPlayer::PlayingState:
         pause();
        break;
    }
}

void Player::stateChanged(QMediaPlayer::State state)
{

    switch (state) {
    case QMediaPlayer::StoppedState:
        mPlaybackStatus=tr("Stopped");
        emit playBackChanged(QString("%1 | %2").arg(m_trackInfo).arg(tr("Stopped")));
        break;
    case QMediaPlayer::PlayingState:
        mPlaybackStatus=tr("Playing");
        emit playBackChanged(QString("%1 | %2").arg(m_trackInfo).arg(tr("Playing")));
        break;
    case QMediaPlayer::PausedState:
         mPlaybackStatus=tr("Paused");
        emit playBackChanged(QString("%1 | %2").arg(m_trackInfo).arg(tr("Paused")));
        break;
    }

}


void Player::setTrackInfo(const QString &info)
{
    m_trackInfo = info;
    if (!m_statusInfo.isEmpty())
        setStatusInfo(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
}


void Player::setPlaybackMode(int value)
{
    QMediaPlaylist::PlaybackMode mode;
    switch (value) {
    case 0:  mode=QMediaPlaylist::CurrentItemOnce;  break;
    case 1:  mode=QMediaPlaylist::CurrentItemInLoop;  break;
    case 2:  mode=QMediaPlaylist::Sequential;  break;
    case 3:  mode=QMediaPlaylist::Loop;  break;
    case 4:  mode=QMediaPlaylist::Random;  break;
    default: mode=QMediaPlaylist::Sequential; break;
    }
    m_playlist->setPlaybackMode(mode);

}
void Player::save(const QString &name)
{

    QString file=D_CACHE+"/"+name;
    QSettings settings(file,QSettings::IniFormat);

    settings.beginWriteArray("Media");

   foreach (QString key,  settings.allKeys()) {
       settings.remove(key);
   }
    int count=  m_playlist->mediaCount();
    for (int i = 0; i < count; ++i) {
        QString url = m_playlist->media(i).canonicalUrl().toString();
        QString title=m_playlistModel->mediaTitle(url);

        settings.setArrayIndex(i);
        settings.setValue("Title", title);
        settings.setValue("Url", url);
    }
    settings.endArray();

    settings.beginGroup("Curent");
    int idx=m_playlist->currentIndex();
    qint64 pos=m_player->position();
    settings.setValue("Index", idx);
    settings.setValue("Pos", pos);
    settings.setValue("Duration", m_player->duration());
    settings.endGroup();


}
void Player::openSavedList(QString name)
{

    QString file=D_CACHE+"/"+name;
    QSettings settings(file,QSettings::IniFormat);
    int count = settings.beginReadArray("Media");


    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);

          QUrl url2 =settings.value("Url").toString();

           qDebug()<<"<<<<<<<<<<<<<<<"<<url2;

        QString title=settings.value("Title").toString();
        m_playlist->addMedia(url2);
        QModelIndex idx=m_playlistModel->index(i,0);
        m_playlistModel->setTitle(idx,url2,title);

    }
    settings.endArray();

    settings.beginGroup("Curent");


    int idx=settings.value("Index",0).toInt();
    qint64 pos=settings.value("Pos",0).toLongLong();


    settings.endGroup() ;
    m_playlist->setCurrentIndex(idx);
    m_player->setPosition(pos);

}

// bool Player::canPlay()
// {
//  //  qDebug()<<!m_playlist->isEmpty();
//         return !m_playlist->isEmpty();

// }
