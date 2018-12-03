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
#include <QSettings>
#include <QLayout>
#include <QMessageBox>

Player::Player(QListView *playlist, QWidget *parent)
    : QWidget(parent),mPlaylistView(playlist)
{

    mPlayerAdaptor=new PlayerAdaptor(this);
    new MainAdaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject(QString("/org/mpris/MediaPlayer2"),QString("Player"), this);
    // connection.registerObject(QString("/org/mpris/MediaPlayer2/Player"),QString("Player"), this);

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
    mControls->setVolume(mPlayer->volume());

    connect(mPlayer, &QMediaPlayer::durationChanged, mControls, &PlayerControls::durationChanged);
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &Player::setduration);

    connect(mPlayer, &QMediaPlayer::positionChanged, mControls, &PlayerControls::positionChanged);
    // connect(mPlayer, &QMediaPlayer::positionChanged, mPlayerAdaptor, &PlayerAdaptor::setPos);
    connect(mPlayerAdaptor, &PlayerAdaptor::Seeked, this, &Player::seek);

    connect(ACtions::instance(), &ACtions::playPause, this, &Player::playPause);
    connect(ACtions::instance(), &ACtions::next,  mPlaylist, &QMediaPlaylist::next);
    connect(ACtions::instance(), &ACtions::previous, this, &Player::previous);
    connect(ACtions::instance(), &ACtions::play, mPlayer, &QMediaPlayer::play);
    connect(ACtions::instance(), &ACtions::Pause, mPlayer, &QMediaPlayer::pause);
    connect(ACtions::instance(), &ACtions::Stop, mPlayer, &QMediaPlayer::stop);
    connect(ACtions::instance(), &ACtions::playbackModeChanged, this,&Player::setPlaybackMode);
    connect(ACtions::instance(), &ACtions::clearList, this,&Player::cleanList);
    connect(ACtions::instance(), &ACtions::removeItem, this,&Player::rmovePlaylistItem);

    connect(mControls, &PlayerControls::changeVolume, mPlayer, &QMediaPlayer::setVolume);
    connect(mControls, &PlayerControls::changeRate, mPlayer, &QMediaPlayer::setPlaybackRate);

    connect(mPlayer, &QMediaPlayer::stateChanged, this, &Player::stateChanged);
    connect(mPlayer, &QMediaPlayer::volumeChanged, mControls, &PlayerControls::setVolume);

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

    QTimer::singleShot(10,this,&Player::openSavedList);

}

Player::~Player()
{
    saveList();
}
//-----------------------------------------------------------------
void Player::rmovePlaylistItem()
{

    QModelIndex idx=mPlaylistView->currentIndex();
    if(!idx.isValid())return;

    int row=idx.row();
    mPlaylist->removeMedia(row);
    //  m_playlistModel->removeRow(idx.row());
    mPlaylistView->setCurrentIndex(mPlaylistModel->index(row,0));
}

//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
void Player::metaDataChanged()
{

    if (! mPlayer->isMetaDataAvailable()) { return ; }

        //---------------------------------------
        QFileInfo fi=    mPlaylist->currentMedia().canonicalUrl().toLocalFile();
        QString   murl=  mPlaylist->currentMedia().canonicalUrl().toString();
        QString   title= mPlayer->metaData(QMediaMetaData::Title).toString();
        QString   album= mPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
        QString   artist=mPlayer->metaData(QMediaMetaData::ContributingArtist).toString();
        QString   albumArtist=mPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
        QString   genre= mPlayer->metaData(QMediaMetaData::Genre).toString();
        //qlonglong   duration= mPlayer->metaData(QMediaMetaData::Size).toLongLong();
        //QString   TrackNumber= mPlayer->metaData(QMediaMetaData::TrackNumber).toString();
        qlonglong lenght=QVariant(mPlayer->duration()).toLongLong();
        QImage    img=mPlayer->metaData(QMediaMetaData::CoverArtImage).value<QImage>();

        //  QString duration=QTime::fromMSecsSinceStartOfDay(QVariant(lenght).toInt()).toString();

        title=title.isEmpty() ? fi.fileName() : title;
        artist=artist.isEmpty()? albumArtist: artist;
        //---------------------------------------
        QString tumbcach=TEMP_CACH+"/"+title+".png";
        mMetaDataMap.clear();
        mMetaDataMap.insert("xesam:title",title);
        mMetaDataMap.insert("xesam:album",album);
        mMetaDataMap.insert("xesam:url",murl);
        mMetaDataMap.insert("xesam:artist",QStringList()<<artist);
        mMetaDataMap.insert("mpris:length",lenght*1000);
        mMetaDataMap.insert("gldfinch:time",lenght);
        mMetaDataMap.insert("mpris:artUrl",tumbcach);

        setCovertImage(img,album);

        QVariantMap map;
        map.insert("Metadata",mMetaDataMap);
        mPlayerAdaptor->propertiesChanged(map);

        //--------------------------------------
        int in=  mPlaylist->currentIndex();

        QModelIndex idx=mPlaylistModel->index(in,0);

        QUrl url= mPlaylist->currentMedia().canonicalUrl();

        mPlaylistModel->setTitle(idx,url,title);

        setTrackInfo(title);

}

//----------------------------------------------------------------------------
void Player::setCovertImage(QImage img,QString album)
{
    //    qDebug()<<"image size:"<<img.width();
    //    qDebug()<<"artSmall  :"<<artSmall;
    //    qDebug()<<"artLarge  :"<<artLarge;
    QFileInfo fi=mPlaylist->currentMedia().canonicalUrl().toLocalFile();

    QString tumbcach=TEMP_CACH+"/"+mMetaDataMap.value("xesam:title").toString()+".png";
    if(!img.isNull()){
        img=img.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        img.save(tumbcach,"PNG");
        emit imageChanged(img);
        return;
    }

        QString path=fi.absolutePath();
        QIcon icon=Tumb::iconAlbum((album.isEmpty() ? fi.absoluteDir().dirName():album),path);
        QString imgPath= Tumb::imageAlbumPath(
                    (album.isEmpty() ? fi.absoluteDir().dirName():album),path);
        img.load(imgPath);
        img=img.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        img.save(tumbcach,"PNG");
        emit imageChanged(img);

}

//----------------------------------------------------------------------------
void Player::previous()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (mPlayer->position() <= 5000)
        mPlaylist->previous();
    else
        mPlayer->setPosition(0);
}

//----------------------------------------------------------------------------
void Player::playLast()
{

    QModelIndex idx=mPlaylistModel->index( mPlaylist->mediaCount()-1,0);
    if (idx.isValid()) {
        mPlaylist->setCurrentIndex(idx.row());
        mPlayer->play();
        metaDataChanged()  ;
        mPlayer->durationChanged(mPlayer->duration());
    }

}

//----------------------------------------------------------------------------
void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        mPlaylist->setCurrentIndex(index.row());
        mPlayer->play();
    }
}

//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
void Player::setSeek(int seconds)
{ mPlayer->setPosition(seconds * 1000);}

//----------------------------------------------------------------------------
void Player::seek(qlonglong Offset)
{ mPlayer->setPosition(Offset /1000);}

//-----------------------------------------------------------------------------
void Player::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus: break;
    case QMediaPlayer::NoMedia:                      break;
    case QMediaPlayer::LoadedMedia:               break;
    case QMediaPlayer::BufferingMedia:             break;
    case QMediaPlayer::BufferedMedia:     setStatusInfo(QString());  break;
    case QMediaPlayer::LoadingMedia:      setStatusInfo(tr("Loading..."));   break;
    case QMediaPlayer::StalledMedia:         setStatusInfo(tr("Media Stalled"));   break;
    case QMediaPlayer::EndOfMedia:        QApplication::alert(this);     break;
    case QMediaPlayer::InvalidMedia:        displayErrorMessage();  break;
    }

}

void Player::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (   status == QMediaPlayer::LoadingMedia   ||
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
    case QMediaPlayer::StoppedState: play();    break;
    case QMediaPlayer::PausedState:   play();    break;
    case QMediaPlayer::PlayingState:    pause(); break;
    }


}

void Player::stateChanged(QMediaPlayer::State state)
{

    QString pStatu;;
    switch (state) {
    case QMediaPlayer::StoppedState:
        ACtions::setPlayIcon();
        mPlaybackStatus=tr("Stopped");
        pStatu="Stopped";
        emit playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(tr("Stopped")));
        break;
    case QMediaPlayer::PlayingState:
        ACtions::setPauseIcon();
        mPlaybackStatus=tr("Playing");
        pStatu="Playing";
        emit playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(tr("Playing")));
        break;
    case QMediaPlayer::PausedState:
        ACtions::setPlayIcon();
        mPlaybackStatus=tr("Paused");
        pStatu="Paused";
        emit playBackChanged(QString("%1 | %2").arg(mTrackInfo).arg(tr("Paused")));
        break;
    }
    //---------------------------------Dbus
    QVariantMap map;
    map["PlaybackStatus"]=pStatu;
    mPlayerAdaptor->propertiesChanged(map);

//    QString title=mMetaDataMap.value("xesam:title").toString();
//    QString tumbcach=TEMP_CACH+"/"+title+".png";


//    if(PlayerAdaptor::Notify( QApplication::applicationName(),
//               tumbcach,
//               mPlaybackStatus,"info",  -1))
//    { return; }
}

//---------------------------------------------------------------------------
void Player::setTrackInfo(const QString &info)
{
    mTrackInfo = info;
    if (!mStatusInfo.isEmpty())
        setStatusInfo(QString("%1 | %2").arg(mTrackInfo).arg(mStatusInfo));
}

//--------------------------------------------------------------------------
void Player::setPlaybackMode(int value)
{
    QMediaPlaylist::PlaybackMode mode;
    switch (value) {
    case P_PlayOne:  mode=QMediaPlaylist::CurrentItemOnce;  break;
    case P_RepeatOne:  mode=QMediaPlaylist::CurrentItemInLoop;  break;
    case P_Squent:  mode=QMediaPlaylist::Sequential;  break;
    case P_Repeat:  mode=QMediaPlaylist::Loop;  break;
    case P_Random:  mode=QMediaPlaylist::Random;  break;
    default: mode=QMediaPlaylist::Sequential; break;
    }
    mPlaylist->setPlaybackMode(mode);

}

//--------------------------------------------------------------------
void Player::saveList()
{

    QSettings settings(APP_NAME,"playlist");
    settings.beginWriteArray("MediaList");
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

//-----------------------------------------------------------------------
void Player::openSavedList()
{

    QSettings settings(APP_NAME,"playlist");
    int count = settings.beginReadArray("MediaList");
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
    mOldPos=settings.value("Pos",0).toLongLong();
    qint64 duration=settings.value("Duration",100).toLongLong();

    settings.endGroup() ;
    mPlaylist->setCurrentIndex(idx);
    setduration(duration);

    if (mOldPos>300) {
        mPlayer->setMuted(true);
        mPlayer->play();
        QTimer::singleShot(700,this,&Player::setseekold);
    }

}

//-----------------------------------------------------------------------------
void Player::setseekold()
{
    mPlayer->pause();
    mPlayer->setMuted(false);
    mPlayer->setPosition(mOldPos);
    mOldPos=0;
}

//----------------------------------------------------------------------------
void Player::setduration(qint64 duration)
{
    mMetaDataMap.insert("mpris:length",duration*1000);
    mMetaDataMap.insert("gldfinch:time",duration);

    QVariantMap map;
    map.insert("Metadata",mMetaDataMap);
    mPlayerAdaptor->propertiesChanged(map);
    mControls->durationChanged(duration);
    QString file=mPlayer->currentMedia().canonicalUrl().toLocalFile();

    QString title=mMetaDataMap.value("xesam:title").toString();

    QModelIndex i=mPlaylistView->currentIndex();
QString info=    mMetaDataMap.value("xesam:artist").toString()+"-"+
        mMetaDataMap.value("xesam:album").toString();
    title=title.isEmpty() ?  mPlaylistModel->data(i ).toString(): title;

    emit titleChanged(title,
                      mMetaDataMap.value("xesam:artist").toString()+"-"+
                      mMetaDataMap.value("xesam:album").toString());

// QString tumbcach=TEMP_CACH+"/"+title+".png";
//  mPlayerAdaptor->Notify(APP_NAME,
//               tumbcach,
//               title,info,  -1);

    // DATABASE
    QString mDur=QTime::fromMSecsSinceStartOfDay(QVariant(duration).toInt()).toString();
    QString dataDur=  DataBase::duration(file);

    if(mDur!=dataDur)
        DataBase::setDuration(mDur,file);

}



