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
#include "database.h"
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
    : QWidget(parent),mPlaylistView(playlist)
{

    new MainAdaptor(this);
    mPlayerAdaptor=new PlayerAdaptor(this);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject(QString("/org/mpris/MediaPlayer2"),QString("Player"), this);
    mPlayer = new QMediaPlayer(this);
    qInfo() << "Supported audio roles:";
    for (QAudio::Role role : mPlayer->supportedAudioRoles())
        qInfo() << "    " << role;
    foreach (QString s, mPlayer->supportedMimeTypes()){
        qInfo() << "    " << s;
    }
    foreach (QString s, mPlayer->supportedCustomAudioRoles()){
        qInfo() << "    " << s;
    }
    // owned by PlaylistModel
    mPlaylist = new QMediaPlaylist();
    mPlayer->setPlaylist(mPlaylist);

    //PlayerAdaptor *mPlayerAdaptor =new PlayerAdaptor(this);
    //mFreeDesktopAdaptor=new FreeDesktopAdaptor(mPlayerAdaptor);

    connect(mPlayer, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &Player::metaDataChanged);
    connect(mPlaylist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);
    connect(mPlayer, &QMediaPlayer::mediaStatusChanged, this, &Player::statusChanged);
    connect(mPlayer, &QMediaPlayer::bufferStatusChanged, this, &Player::bufferingProgress);
    connect(mPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &Player::displayErrorMessage);


    mPlaylistModel = new PlaylistModel(this);
    mPlaylistModel->setPlaylist(mPlaylist);

    mPlaylistView->setModel(mPlaylistModel);
    mPlaylistView->setCurrentIndex(mPlaylistModel->index(mPlaylist->currentIndex(), 0));

    connect(mPlaylistView, &QAbstractItemView::activated, this, &Player::jump);


    controls = new PlayerControls(this);
    controls->setState(mPlayer->state());
    controls->setVolume(mPlayer->volume());

    connect(mPlayer, &QMediaPlayer::durationChanged, controls, &PlayerControls::durationChanged);
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &Player::setduration);

    connect(mPlayer, &QMediaPlayer::positionChanged, controls, &PlayerControls::positionChanged);
    // connect(mPlayer, &QMediaPlayer::positionChanged, mPlayerAdaptor, &PlayerAdaptor::setPos);
    //   connect(this, &Player::propertiesChanged, mFreeDesktopAdaptor, &FreeDesktopAdaptor::PropertiesChanged);

    connect(controls, &PlayerControls::play, mPlayer, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, mPlayer, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, mPlayer, &QMediaPlayer::stop);
    connect(controls, &PlayerControls::next, mPlaylist, &QMediaPlaylist::next);
    connect(controls, &PlayerControls::previous, this, &Player::previous);
    connect(controls, &PlayerControls::changeVolume, mPlayer, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeRate, mPlayer, &QMediaPlayer::setPlaybackRate);
   // connect(controls, &PlayerControls::seek, this, &Player::seek);

    connect(this, &Player::iconsChanged, controls, &PlayerControls::setupIcons);
    //   connect(this, &Player::canPlay, controls, &PlayerControls::setPlayEnabled);
    //  connect(this, &Player::canNext, controls, &PlayerControls::setNextEnabled);
    //  connect(this, &Player::canPrev, controls, &PlayerControls::setPrevEnabled);

    connect(mPlayer, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(mPlayer, &QMediaPlayer::stateChanged, this, &Player::stateChanged);
    connect(mPlayer, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
    //  connect(mPlayer, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
    // connect(m_player, &QMediaPlayer::metaDataChanged,this, &Player::metaDataChanged);

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
        mPlaylistView->setEnabled(false);
    }

    openSavedList("playlist");
    //  controls->setEnabled(false);
    //  metaDataChanged();


    //  QObject::connect(w.player(),&Player::seek,p,&PlayerAdaptor::setProperty)
    //new FreeDesktopAdaptor(w.player());

}

Player::~Player()
{
    save("playlist");
}
//-----------------------------------------------------------------
void Player::rmovePlaylistItem(QModelIndex idx)
{
    int row=idx.row();
    mPlaylist->removeMedia(row);
    //  m_playlistModel->removeRow(idx.row());
    mPlaylistView->setCurrentIndex(mPlaylistModel->index(row,0));
}

void Player::moveMedia(int from,int to)
{

    if(mPlaylist->moveMedia(from,to))
        mPlaylistView->setCurrentIndex(mPlaylistModel->index(to,0));
}

void Player::cleanList()
{

    mPlaylist->clear();
    mPlaylistModel->clear();
}

//-----------------------------------------------------------
bool Player::isPlayerAvailable() const
{
    return mPlayer->isAvailable();
}

void Player::addToPlaylist( QList<QVariantMap> &urls)
{
    QList<QMap<QString,QUrl>> list;
    QMap<QString,QUrl> map;

    int count=mPlaylist->mediaCount();

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
            mPlaylist->addMedia(url);
        }else{

            url.setUrl(QUrl::fromLocalFile(str).toString());
            mPlaylist->addMedia(url);
        }

        QString tit=map.value("Title").toString();
        QModelIndex idx=mPlaylistModel->index(count+i,0);
        mPlaylistModel->setTitle(idx,url,tit);

    }

}
void Player::addToPlaylist( QList<QUrl> &urls)
{
    for (auto &url: urls) {
        mPlaylist->addMedia(url);
    }
}


void Player::metaDataChanged()
{
    if (mPlayer->isMetaDataAvailable()) {

        //---------------------------------------
        QFileInfo fi=    mPlaylist->currentMedia().canonicalUrl().toLocalFile();
        QString   murl=  mPlaylist->currentMedia().canonicalUrl().toString();
        QString   title= mPlayer->metaData(QMediaMetaData::Title).toString();
        QString   album= mPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
        QString   artist=mPlayer->metaData(QMediaMetaData::ContributingArtist).toString();
        QString   albumArtist=mPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
        QString   genre= mPlayer->metaData(QMediaMetaData::Genre).toString();
        qlonglong lenght=QVariant(mPlayer->duration()).toLongLong();
        QImage    img=mPlayer->metaData(QMediaMetaData::CoverArtImage).value<QImage>();

        QString duration=QTime::fromMSecsSinceStartOfDay(QVariant(lenght).toInt()).toString();

        title=title.isEmpty() ? fi.fileName() : title;
        artist=artist.isEmpty()? albumArtist: artist;

        //---------------------------------------
        mMetaDataMap.clear();
        mMetaDataMap.insert("xesam:title",title);
        mMetaDataMap.insert("xesam:album",album);
        mMetaDataMap.insert("xesam:url",murl);
        mMetaDataMap.insert("xesam:artist",QStringList()<<artist);
        mMetaDataMap.insert("mpris:length",lenght);
        mMetaDataMap.insert("mpris:mpris:artUrl",D_CACHE+"/thumb.jpg");

        //---------------------------------Dbus
        QVariantMap map;
        map.insert("Metadata",mMetaDataMap);
        mPlayerAdaptor->propertiesChanged(map);
        //---------------------------------------
        QString strTit=title.isEmpty() ? fi.fileName() : title;
        QString strInf=QString("%1 - %2").arg(artist).arg(album);

        emit titleChanged(title);
        emit infoChanged(strInf);

        //---------------------------------------
        //        QString artSmall= m_player->metaData(QMediaMetaData::CoverArtUrlSmall).toString();
        //        QString artLarge=m_player->metaData(QMediaMetaData::CoverArtUrlLarge).toString();

        //--------------------------------------
        int in=  mPlaylist->currentIndex();
        QModelIndex idx=mPlaylistModel->index(in,0);
        QUrl url= mPlaylist->currentMedia().canonicalUrl();
        mPlaylistModel->setTitle(idx,url,title);
        setTrackInfo(title);

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
        img.save(D_CACHE+"/thumb.jpg");
        emit imageChanged(img);
        return;
    }

    QFileInfo fi=mPlaylist->currentMedia().canonicalUrl().toLocalFile();

    QString path=fi.absolutePath();
    //  TODO: fix title album
    QIcon icon=Tumb::iconAlbum((album.isEmpty() ? fi.absoluteDir().dirName():album),path);
    QString imgPath= Tumb::imageAlbumPath(
                (album.isEmpty() ? fi.absoluteDir().dirName():album),path);
    img.load(imgPath);
    img.save(D_CACHE+"/thumb.jpg");
    emit imageChanged(img);

}

void Player::previous()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (mPlayer->position() <= 5000)
        mPlaylist->previous();
    else
        mPlayer->setPosition(0);
}
void Player::playLast()
{

    QModelIndex idx=mPlaylistModel->index( mPlaylist->mediaCount()-1,0);
    if (idx.isValid()) {
        mPlaylist->setCurrentIndex(idx.row());
        mPlayer->play();
    }

}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        mPlaylist->setCurrentIndex(index.row());
        mPlayer->play();
    }
}

void Player::playlistPositionChanged(int currentItem)
{
    mPlaylistView->setCurrentIndex(mPlaylistModel->index(currentItem, 0));

    //---------------------------------Dbus
    QVariantMap map;
    map.insert("CanGoNext",canGoNext());
    map.insert("CanGoPrevious",canGoPrevious());
    map.insert("CanPlay",mPlayer->isAvailable());

    mPlayerAdaptor->propertiesChanged(map);
}

void Player::setSeek(int seconds)
{ mPlayer->setPosition(seconds * 1000);}



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
    mStatusInfo = info;
    if (!mStatusInfo.isEmpty())
        //    setStatusTip()
        emit  playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(mStatusInfo));
    else{
        emit   playBackChanged(mTrackInfo);

        stateChanged(mPlayer->state());
    }

    //emit playBackChanged
}

void Player::displayErrorMessage()
{
    emit  playBackChanged(mPlayer->errorString());
}

void Player::playPause()
{
    switch (mPlayer->state()) {
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
    QString pStatu;;
    switch (state) {
    case QMediaPlayer::StoppedState:
        mPlaybackStatus=tr("Stopped");
        pStatu="Stopped";
        emit playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(tr("Stopped")));
        break;
    case QMediaPlayer::PlayingState:
        mPlaybackStatus=tr("Playing");
        pStatu="Playing";
        emit playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(tr("Playing")));
        break;
    case QMediaPlayer::PausedState:
        mPlaybackStatus=tr("Paused");
        pStatu="Paused";
        emit playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(tr("Paused")));
        break;
    }
    //---------------------------------Dbus
    QVariantMap map;
    map["PlaybackStatus"]=pStatu;
    mPlayerAdaptor->propertiesChanged(map);
}


void Player::setTrackInfo(const QString &info)
{
    mTrackInfo = info;
    if (!mStatusInfo.isEmpty())
        setStatusInfo(QString("%1 | %2").arg(mTrackInfo).arg(mStatusInfo));
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
    mPlaylist->setPlaybackMode(mode);

}

void Player::save(const QString &name)
{

    QString file=D_CACHE+"/"+name;
    QSettings settings(file,QSettings::IniFormat);

    settings.beginWriteArray("Media");

    foreach (QString key,  settings.allKeys()) {
        settings.remove(key);
    }
    int count=  mPlaylist->mediaCount();
    for (int i = 0; i < count; ++i) {
        QString url = mPlaylist->media(i).canonicalUrl().toString();
        QString title=mPlaylistModel->mediaTitle(url);

        settings.setArrayIndex(i);
        settings.setValue("Title", title);
        settings.setValue("Url", url);
    }
    settings.endArray();

    settings.beginGroup("Curent");
    int idx=mPlaylist->currentIndex();
    qint64 pos=mPlayer->position();
    settings.setValue("Index", idx);
    settings.setValue("Pos", pos);
    settings.setValue("Duration", mPlayer->duration());
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


        QString title=settings.value("Title").toString();
        mPlaylist->addMedia(url2);
        QModelIndex idx=mPlaylistModel->index(i,0);
        mPlaylistModel->setTitle(idx,url2,title);

    }
    settings.endArray();

    settings.beginGroup("Curent");


    int idx=settings.value("Index",0).toInt();
    qint64 pos=settings.value("Pos",0).toLongLong();
    //qint64 duration=settings.value("Duration",100).toLongLong();

    settings.endGroup() ;
    mPlaylist->setCurrentIndex(idx);
    mPlayer->setPosition(pos);
    metaDataChanged();

}

void Player::setduration(qint64 duration)
{
    QString file=mPlayer->currentMedia().canonicalUrl().toLocalFile();
    QString mDur=QTime::fromMSecsSinceStartOfDay(QVariant(duration).toInt()).toString();
    QString dataDur=  DataBase::duration(file);
    qDebug()<<" Player::durationChanged"<<mDur<<dataDur;
    if(mDur!=dataDur)
        DataBase::setDuration(mDur,file);


}

bool Player::canGoNext()
{
    //  return m_playlist->currentIndex()<(m_playlist->mediaCount()-1);
    int idx=mPlaylist->currentIndex();
    if(idx<(mPlaylist->mediaCount()-1))
        return true;
    return false;

}
bool Player::canGoPrevious()
{
    int idx=mPlaylist->currentIndex();
    if(idx>0)
        return true;

    return false;

}
