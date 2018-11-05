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


    mControls = new PlayerControls(this);
    mControls->setState(mPlayer->state());
    mControls->setVolume(mPlayer->volume());

    connect(mPlayer, &QMediaPlayer::durationChanged, mControls, &PlayerControls::durationChanged);
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &Player::setduration);

    connect(mPlayer, &QMediaPlayer::positionChanged, mControls, &PlayerControls::positionChanged);
    // connect(mPlayer, &QMediaPlayer::positionChanged, mPlayerAdaptor, &PlayerAdaptor::setPos);
    //   connect(this, &Player::propertiesChanged, mFreeDesktopAdaptor, &FreeDesktopAdaptor::PropertiesChanged);

    connect(mControls, &PlayerControls::play, mPlayer, &QMediaPlayer::play);
    connect(mControls, &PlayerControls::pause, mPlayer, &QMediaPlayer::pause);
    connect(mControls, &PlayerControls::stop, mPlayer, &QMediaPlayer::stop);
    connect(mControls, &PlayerControls::next, mPlaylist, &QMediaPlaylist::next);
    connect(mControls, &PlayerControls::previous, this, &Player::previous);
    connect(mControls, &PlayerControls::changeVolume, mPlayer, &QMediaPlayer::setVolume);
    connect(mControls, &PlayerControls::changeRate, mPlayer, &QMediaPlayer::setPlaybackRate);
   // connect(controls, &PlayerControls::seek, this, &Player::seek);

    connect(this, &Player::iconsChanged, mControls, &PlayerControls::setupIcons);
    //   connect(this, &Player::canPlay, controls, &PlayerControls::setPlayEnabled);
    //  connect(this, &Player::canNext, controls, &PlayerControls::setNextEnabled);
    //  connect(this, &Player::canPrev, controls, &PlayerControls::setPrevEnabled);

    connect(mPlayer, &QMediaPlayer::stateChanged, mControls, &PlayerControls::setState);
    connect(mPlayer, &QMediaPlayer::stateChanged, this, &Player::stateChanged);
    connect(mPlayer, &QMediaPlayer::volumeChanged, mControls, &PlayerControls::setVolume);
    //  connect(mPlayer, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
    // connect(m_player, &QMediaPlayer::metaDataChanged,this, &Player::metaDataChanged);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    hLayout->addSpacing(3);
    hLayout->addWidget(mControls);
    hLayout->addSpacing(3);


    setLayout(hLayout);

    if (!mPlayer->isAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        mControls->setEnabled(false);
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

//--  Add String list to playlist
void Player::addToPlaylist( QList<QVariantMap> &files)
{
    QList<QMap<QString,QUrl>> list;
    QMap<QString,QUrl> map;

    int count=mPlaylist->mediaCount();

    for (int i = 0; i < files.count(); ++i) {
        QMap<QString,QVariant> map=files.at(i);


        QString str=map.value("Url").toString();
         //TODO : fix this Url
          QUrl url;
        if(str.contains("://")){ url.setUrl(str); }
        else{url.setUrl(QUrl::fromLocalFile(str).toString());}

        if(url.isValid()){
            mPlaylist->addMedia(url);

            QString tit=map.value("Title").toString();
            QModelIndex idx=mPlaylistModel->index(count+i,0);
            mPlaylistModel->setTitle(idx,url,tit);
        }
    }

}

//--  Add Urls to playlist
void Player::addToPlaylist( QList<QUrl> &urls)
{
    for (auto &url: urls) {
        mPlaylist->addMedia(url);
    }
}

//--  Add file string to playlist
void Player::setFile(const QString &file)
{
     QUrl url=QUrl::fromLocalFile(file);
      mPlaylist->addMedia(url);
      playLast();
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


       // emit infoChanged(strInf);

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

        qDebug()<<url2.scheme()<<url2.isLocalFile()<<url2;

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
        emit titleChanged(mMetaDataMap.value("xesam:title").toString(),
                          mMetaDataMap.value("xesam:artist").toString()+"-"+
                          mMetaDataMap.value("xesam:album").toString());
    QString file=mPlayer->currentMedia().canonicalUrl().toLocalFile();
    QString mDur=QTime::fromMSecsSinceStartOfDay(QVariant(duration).toInt()).toString();
    QString dataDur=  DataBase::duration(file);
    qDebug()<<" Player::durationChanged"<<mDur<<dataDur;
    if(mDur!=dataDur)
        DataBase::setDuration(mDur,file);


}

bool Player::canGoNext()
{
    if(mPlaylist->currentIndex()<(mPlaylist->mediaCount()-1))
        return true;

    return false;
}

bool Player::canGoPrevious()
{
    if(mPlaylist->currentIndex()>0)
        return true;

    return false;
}
