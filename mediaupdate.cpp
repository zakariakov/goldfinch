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

#include "mediaupdate.h"
#include "dialogoptions.h"
#include <QFileInfo>
#include <QSettings>
#include <QMediaMetaData>
#include <QTime>
#include <QApplication>
#include <QDirIterator>
#include <QTextCodec>
#include <QProcess>
#include <QDebug>
MediaUpdate::MediaUpdate( QObject *parent)
    : QObject(parent)
{
    DataBase::openDataBase();
    mWatcher=new QFileSystemWatcher;
    // mWatcher->addPath("/media/Data/Music");
    //! [create-objs]
    mPlayer = new QMediaPlayer(this);
    connect(mPlayer, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));

    // owned by PlaylistModel
    mPlaylist = new QMediaPlaylist();
    //addDirectory();
    mPlayer->setPlaylist(mPlaylist);

    mPlaylist->setCurrentIndex(0);

    // thread
    mThread=new Thread;
    //  connect(mThread,SIGNAL(removelast()),this,SLOT(removelast()));

    connect(mThread,SIGNAL(removeKey(QString)),this,SLOT(removelast(QString)));
    connect(mThread,SIGNAL(finished())         ,this,SLOT(startNewThread()));
    //    connect(mThread,SIGNAL(terminated(QString)),this,SIGNAL(updateThumbnail(QString)));

    //    QFile file(D_CACHE+"/Directory");
    //    if (file.open(QFile::ReadOnly )){
    //    QTextStream textStream(&file);
    //    textStream.setCodec(QTextCodec::codecForName("UTF-8"));
    //    QString line ;//premier line;

    //    while   (!textStream.atEnd()) {
    //        line = textStream.readLine().trimmed();
    //        mWatcher->addPath(line.trimmed());
    //    }
    //}

    // تحميل المجلدات المستخدمة لمراقبنها في حالة التغير
    chargeDirectoryWatcher();
    //qDebug()<<mWatcher->directories();
    connect(mWatcher,&QFileSystemWatcher::directoryChanged,this,&MediaUpdate::directoryChanged);

}


MediaUpdate::~MediaUpdate()
{

}


//--------------------------------------------------------------------
void MediaUpdate::chargeDirectoryWatcher()
{
    QSettings settings;
    int count = settings.beginReadArray("Directory");
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        QString path =settings.value("Dir").toString();

        mWatcher->addPath(path);

        QDirIterator itDir(path,QDir::AllDirs| QDir::NoDotAndDotDot|QDir::NoSymLinks,
                           QDirIterator::Subdirectories);
        while (itDir.hasNext()) {
            QString dir= itDir.next();
            //   qDebug()<<"isdir>>>>>>>>>>>>>>>>>>"<<dir;
            mWatcher->addPath(dir);
        }
    }
}

//--------------------------------------------------------------------
// الملفات المعتمدة
QStringList supportedMimeTypes()
{
    return  QStringList()<<"*.mp4"<<"*.wmv"<<"*.avi"<<"*.ogv"<<"*.mov"
                        <<"*.mp3"<<"*.ogg"<<"*.wma"<<"*.wav"<<"*.aac"
                       <<"*.opus"<<"*.amr"<<"*.m4a";
}

//--------------------------------------------------------------------
//  جلب مدة الملف الصوتي من برامج خارجية في حالة عدم تحميله
QString getDuration(QString filePath)
{
    QStringList list2;
    list2<<"-i"<<filePath<<"-hide_banner";
    QProcess p;
    //-------------------------------------------
    p.start("ffmpeg",list2);
    if (!p.waitForStarted()) {   return QString() ;  }

    if (!p.waitForFinished()){   return QString();   }

    QString error=p.readAllStandardError();

    if(error.isEmpty())return QString();

    QStringList list=error.split("\n");
    QString title, album ,artist ,genre;

    foreach (QString s, list) {
        //       if(s.trimmed().startsWith("title")){
        //         artist= s.section(":",1,1).trimmed();
        //         qDebug()<<"ffmpeg title:"<<title;
        //       }
        //       if(s.trimmed().startsWith("album")){
        //         album= s.section(":",1,1).trimmed();
        //         qDebug()<<"ffmpeg album:"<<album;
        //       }
        //       if(s.trimmed().startsWith("artist")){
        //         artist= s.section(":",1,1).trimmed();
        //         qDebug()<<"ffmpeg artist:"<<artist;
        //       }
        //       if(s.trimmed().startsWith("genre")){
        //         artist= s.section(":",1,1).trimmed();
        //         qDebug()<<"ffmpeg genre:"<<genre;
        //       }
        if(s.trimmed().startsWith("Duration")){
            s=s.remove("Duration:");
            QString name=s.section(",",0,0);
            return name.section(".",0,0).trimmed();
        }

    }
    return QString();
}




//![0]--
//--------------------------------------------------------------------
void MediaUpdate::metaDataChanged()
{
    QUrl url=mPlayer->currentMedia().canonicalUrl();
    qDebug()<<"mPlaylist->url0()"<<url;
    if (mPlayer->isMetaDataAvailable()) {

        QUrl url=mPlayer->currentMedia().canonicalUrl();
        qDebug()<<"mPlaylist->url()"<<url;
        QFileInfo fi(mPlayer->currentMedia().canonicalUrl().toLocalFile());
        QString path=fi.absoluteFilePath();
        if(!fi.exists())
            path=url.toString();

        QString artist=(mPlayer->metaData(QMediaMetaData::ContributingArtist).toString());
        QString albumArtist=(mPlayer->metaData(QMediaMetaData::AlbumArtist).toString());
        QString title=(mPlayer->metaData(QMediaMetaData::Title).toString());
        QString genre=(mPlayer->metaData(QMediaMetaData::Genre).toString());
        QString album=(mPlayer->metaData(QMediaMetaData::AlbumTitle).toString());
        QString mediaSize=(mPlayer->metaData(QMediaMetaData::Size).toString());

        QString duration=QTime::fromMSecsSinceStartOfDay(QVariant(mPlayer->duration()).toInt()).toString();
        //        if(duration.isEmpty())
        //            duration=  getDuration(path).trimmed();

        //   qDebug()<<"MediaUpdate"<<title<<"duration:"<<time;
        if(artist.isEmpty())artist=albumArtist;
        album=!album.isEmpty() ? album: fi.dir().dirName();

        QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
        s.setValue(album,fi.absolutePath());

        //        DataBase::addNewSong(!title.isEmpty() ? title: fi.fileName(),
        //                             !artist.isEmpty() ? artist: tr("Unknown"),
        //                             !album.isEmpty() ? album: fi.dir().dirName(),
        //                             !genre.isEmpty() ? genre:tr("Unknown") ,
        //                             path,
        //                             !duration.isEmpty()? duration:"00:00:00"
        //                                                  );

        QVariantMap map;
        map["title"]=!title.isEmpty() ? title: fi.fileName();
        map["artist"]=!artist.isEmpty() ? artist: tr("Unknown");
        map["album"]=!album.isEmpty() ? album: fi.dir().dirName();
        map["genre"]=!genre.isEmpty() ? genre:tr("Unknown");
        map["path"]=path;  //TODO FIX URL
        map["duration"]=duration;
        // Thread
        updateFile(map,path);
        qDebug()<<"mPlaylist->currentIndex()"<<mPlaylist->currentIndex();
        mPlaylist->next();

        emit progressValueChanged(mPlaylist->currentIndex());
        qApp->processEvents();


    }else{
        //  mPlaylist->next();
    }

    if(mPlaylist->currentIndex()==-1){
        emit updated();
        mPlaylist->clear();
        chargeDirectoryWatcher();
    }
}

//--------------------------------------------------------------------
//![0] from mainwindow
void MediaUpdate::getDirListOptions()
{
    DialogOptions *dlg=new DialogOptions;
    if(dlg->exec()==QDialog::Accepted){
        //
        bool clear=dlg->clearData();
        if(clear){
            if(DataBase::clearDatabase()){
                QFile::remove(D_CACHE+"/filesinfo");
                emit updated();
            }
        }
        //
        addUpdateDirectories(false);
    }

    delete dlg;
}

//--------------------------------------------------------------------
//![1] from getDirListOptions
void MediaUpdate::addUpdateDirectories(bool all)
{
    QStringList list;

    mPlaylist->clear();
    //---------------------------------------

    QSettings settings;
    settings.sync();

    int count = settings.beginReadArray("Directory");
    if(count==0){ list.append(D_DMUSIC);  }

    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        QString dir =settings.value("Dir").toString();


        if(!all){
            bool checked=settings.value("Checked").toBool();
            if(checked)  list.append(dir);
        }


    }

    settings.endArray();
    //---------------------------------------
    // Scan directory
    // and add media to plylist.
    foreach (QString path, list) {scanDirectory(path);}

    // after scanning if mediacount >0 then
    // start extract metadata and adding to database
    if(mPlaylist->mediaCount()>0){
        emit  progressMaxChanged(mPlaylist->mediaCount());
        mPlaylist->setCurrentIndex(0);
    }

}

//--------------------------------------------------------------------
//![1] from addUpdateDirectory
void MediaUpdate::scanDirectory(const QString &path)
{
    //  مسح المحلدات والمجلدات الفرعية لجلب ملفات
    QDirIterator it(path,supportedMimeTypes(),QDir::Files| QDir::NoDotAndDotDot|QDir::NoSymLinks,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString file= it.next();
        if(  !DataBase::fileExist(file)){
            QUrl url(QUrl::fromLocalFile(file));
            mPlaylist->addMedia(url);
        }
    }

}

//--------------------------------------------------------------------
//![0] from signal watcher to mainwindow
void MediaUpdate::directoryChanged(const QString &path)
{
    mLisUpdateDirs.append(path);
    emit directoryNeedUpdate(true);
}

//--------------------------------------------------------------------
//![1] from mainwindow
void MediaUpdate::setUpdateDirs(bool update)
{

    if(!update){
        mLisUpdateDirs.clear();
        return;
    }

    foreach (QString path, mLisUpdateDirs) {
        updateDirectory(path);
    }
    mLisUpdateDirs.clear();
}


//--------------------------------------------------------------------
//![2] from mainwindow and  setUpdateDirs
void MediaUpdate::updateDirectory(const QString &dir)
{

    mPlaylist->clear();

    QDirIterator it(dir,supportedMimeTypes(),QDir::Files| QDir::NoDotAndDotDot |QDir::NoSymLinks);
    while (it.hasNext()) {
        QString file= it.next();
        if(  !DataBase::fileExist(file)){
            QUrl url(QUrl::fromLocalFile(file));
            mPlaylist->addMedia(url);
        }
    }

    if(mPlaylist->mediaCount()>0){
        emit  progressMaxChanged(mPlaylist->mediaCount());
        mPlaylist->setCurrentIndex(0);
    }

}

//--------------------------------------------------------------------
//! add new files to database from mainwidow
void MediaUpdate::addFiles(const QList<QUrl>urls)
{

    mPlaylist->clear();
    for (auto &url: urls) {
        QString file=url.toLocalFile();
        if( !DataBase::fileExist(file)){
            mPlaylist->addMedia(url);
        }
    }

    if(mPlaylist->mediaCount()>0)
        mPlaylist->setCurrentIndex(0);
}

// ******************** THREAD **************************
//--------------------------------------------------------------------THREAD
void MediaUpdate::removelast(const QString &key){

    mMapTread.remove(key);
}

//![1] from metadata
//--------------------------------------------------------------------THREAD
void  MediaUpdate::updateFile(QVariantMap map,const QString &path)
{
    if(mMapTread.contains(path))return;

    mMapTread.insert(path,map);
    startNewThread();
}

//--------------------------------------------------------------------THREAD
void MediaUpdate::startNewThread()
{
    if(mThread->isRunning())return;

    if(mMapTread.isEmpty()) {
        return;
    }

    QVariantMap map=mMapTread.last();
    mThread->setFile(map);
    mThread->start();

}

//--------------------------------------------------------------------THREAD
Thread:: Thread()
{


}

//--------------------------------------------------------------------THREAD
void Thread::run()
{

    QString path= mMap.value("path").toString();
    QString duration=  mMap.value("duration").toString();
    if(duration.isEmpty())
        duration=  getDuration(path).trimmed();
    qDebug()<<"Thread::run()::"<<duration<<path;
    DataBase::addNewSong(mMap.value("title").toString() ,
                         mMap.value("artist").toString(),
                         mMap.value("album").toString() ,
                         mMap.value("genre").toString() ,
                         path,
                         !duration.isEmpty()? duration:"00:00:00" );

    emit removeKey(path);

}
