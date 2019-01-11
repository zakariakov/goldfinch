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
#include "setting.h"
#include <QFileInfo>
#include <QSettings>
//#include <QMediaMetaData>
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

    mThread=new Thread;
    //  connect(mThread,SIGNAL(removelast()),this,SLOT(removelast()));

    connect(mThread,SIGNAL(removeKey(QString)),this,SLOT(removelast(QString)));
    connect(mThread,SIGNAL(finished())         ,this,SLOT(startNewThread()));
       // تحميل المجلدات المستخدمة لمراقبنها في حالة التغير
    chargeDirectoryWatcher();

    connect(mWatcher,&QFileSystemWatcher::directoryChanged,this,&MediaUpdate::directoryChanged);
connect(TagId::instance(),&TagId::updateFile,this,&MediaUpdate::addupdates);
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
QString getffmpegDuration(QString filePath)
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



/*
void MediaUpdate::showSettings()
{
    DialogOptions *dlg=new DialogOptions;
    if(dlg->exec()==QDialog::Accepted){
        //
        bool clear=dlg->clearData();
        if(clear){
            if(DataBase::clearDatabase()){
                QFile::remove(CACHE+"/filesinfo");
                emit updated();
            }
        }
        //
        addUpdateDirectories(false);
    }

    delete dlg;
}
*/
void MediaUpdate::updateAllDirectories()
{
    addUpdateDirectories(true);
}
//--------------------------------------------------------------------
//![1] from getDirListOptions
void MediaUpdate::addUpdateDirectories(bool all)
{
    qDebug()<< "MediaUpdate::addUpdateDirectories"<<all;
    QStringList list;

    QSettings settings;
    settings.sync();

    int count = settings.beginReadArray("Directory");
    if(count==0){ list.append(D_DMUSIC);  }

    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        QString dir =settings.value("Dir").toString();


        if(all){
            list.append(dir);
        }else{
            bool checked=settings.value("Checked").toBool();
            if(checked)  list.append(dir);
        }


    }

    settings.endArray();
    //---------------------------------------
    // Scan directory
    foreach (QString path, list) {scanDirectory(path);}

    // after scanning if mediacount >0 then
    // start extract metadata and adding to database

    if(listFiles.count()>0){
        emit  progressMaxChanged(listFiles.count());
        addupdates();
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

            listFiles.append(file);

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

   // mPlaylist->clear();

    QDirIterator it(dir,supportedMimeTypes(),QDir::Files| QDir::NoDotAndDotDot |QDir::NoSymLinks);
    while (it.hasNext()) {
        QString file= it.next();
            listFiles.append(file);
    }

    if(listFiles.count()>0){
        emit  progressMaxChanged(listFiles.count());
        addupdates();
    }

}

//--------------------------------------------------------------------
//! add new files to database from mainwidow
void MediaUpdate::addFiles(const QList<QUrl>urls)
{

    for (auto &url: urls) {
        QString file=url.toLocalFile();
         listFiles.append(file);
    }

    if(listFiles.count()>0){
        emit  progressMaxChanged(listFiles.count());
        addupdates();
    }
}

// ******************** THREAD **************************
//--------------------------------------------------------------------THREAD
void MediaUpdate::removelast(const QString &key){

    int count=listFiles.count();
    listFiles.removeAll(key);

    emit progressValueChanged(count);
    qApp->processEvents();

    if(listFiles.count()<1){
        emit updated();
        chargeDirectoryWatcher();
    }
}

void  MediaUpdate::addupdates(const QString file)
{

    if(!listFiles.contains(file)&& !file.isEmpty())
       listFiles.append(file);

    startNewThread();
}

//--------------------------------------------------------------------THREAD
void MediaUpdate::startNewThread()
{
    if(mThread->isRunning())return;

    if(listFiles.isEmpty()) {
        emit updated();
        chargeDirectoryWatcher();
        return;
    }

    mThread->setFile(listFiles.last());
    mThread->start();

}

//--------------------------------------------------------------------THREAD
Thread:: Thread()
{
}

//--------------------------------------------------------------------THREAD
void Thread::run()
{

   if(  !DataBase::fileExist(mFile)){

         //-----------------------------------------------------------------             GET METADATA
       QVariantMap map=TagId::mediaTags(mFile);
       //----------------------------------------------------------------- GET DURATION FFMPEG
       QString duration= map.value(COL_S_TIME).toString();
       if(duration=="00:00:00")
           duration=  getffmpegDuration(mFile).trimmed();
       //-----------------------------------------------------------------          SAVE TO DATABASE
       DataBase::addNewSong(map.value(COL_S_TITLE).toString() ,
                            map.value(COL_S_ARTIST).toString(),
                            map.value(COL_S_ALBUM).toString() ,
                            map.value(COL_S_GENRE).toString() ,
                            mFile,
                            duration );
       //-------------------------------------------------------------                  SAVE ALBUM DIR
       QFileInfo fi(mFile);
//       QSettings s(CACHE+"/albums",QSettings::IniFormat);
//       s.setValue( map.value(COL_S_ALBUM).toString(),fi.absolutePath());
      // Setting::setAlbumImgPath( map.value(COL_S_ALBUM).toString(),fi);
        DataBase::addNewAlbumImgPath(map.value(COL_S_ALBUM).toString(),fi.absolutePath());
   }


    emit removeKey(mFile);

}
