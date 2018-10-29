#include "mediaupdate.h"
#include <QFileInfo>
#include <QSettings>
#include <QMediaMetaData>
#include <QTime>
#include <QApplication>
#include <QDirIterator>
#include <QTextCodec>
#include <QProcess>
MediaUpdate::MediaUpdate( QObject *parent)
    : QObject(parent)
{

    mWatcher=new QFileSystemWatcher;
    // mWatcher->addPath("/media/Data/Music");
    //! [create-objs]
    player = new QMediaPlayer(this);
    connect(player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));

    // owned by PlaylistModel
    playlist = new QMediaPlaylist();
    //addDirectory();
    player->setPlaylist(playlist);

    playlist->setCurrentIndex(0);

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
    list2<<"-i"<<filePath;
    QProcess p;
    //-------------------------------------------
    p.start("ffmpeg",list2);
    if (!p.waitForStarted()) {   return QString() ;  }

    if (!p.waitForFinished()){   return QString();   }

    QString error=p.readAllStandardError();

    if(error.isEmpty())return QString();

    QStringList list=error.split("\n");

    foreach (QString s, list) {
        if(s.trimmed().startsWith("Duration")){
            s=s.remove("Duration:");
            QString name=s.section(",",0,0);
            return name.section(".",0,0).trimmed();

        }

    }
    return QString();
}

//--------------------------------------------------------------------
void MediaUpdate::directoryChanged(const QString &path)
{
    mLisUpdateDirs.append(path);

    //TODO ADD Message for Adding dir Updat
    //   qDebug()<<" MediaUpdate::directoryChanged ::: "<<path;
    //  updateDirectory(path);
    emit dirNidUpdate(true);


}

//--------------------------------------------------------------------
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


//void MediaUpdate::saveDirectory()
//{
//    QFile data(D_CACHE+"/Directory");
//    if (data.open(QFile::WriteOnly )) {
//        QTextStream out(&data);
//        out.setCodec(QTextCodec::codecForName("UTF-8"));
//        foreach (QString path, mWatcher->directories()) {
//            qDebug()<<"MediaUpdate::saveDirectory()"<<path;
//            out << path << "\n" ;
//        }
//    }
//}

//--------------------------------------------------------------------
void MediaUpdate::metaDataChanged()
{

    if (player->isMetaDataAvailable()) {

        QUrl url=player->currentMedia().canonicalUrl();

        QFileInfo fi(player->currentMedia().canonicalUrl().toLocalFile());
        QString path=fi.absoluteFilePath();
        if(!fi.exists())
            path=url.toString();

        QString artist=(player->metaData(QMediaMetaData::ContributingArtist).toString());
        QString albumArtist=(player->metaData(QMediaMetaData::AlbumArtist).toString());
        QString title=(player->metaData(QMediaMetaData::Title).toString());
        QString genre=(player->metaData(QMediaMetaData::Genre).toString());
        QString album=(player->metaData(QMediaMetaData::AlbumTitle).toString());
        QString mediaSize=(player->metaData(QMediaMetaData::Size).toString());

        QString duration=QTime::fromMSecsSinceStartOfDay(QVariant(player->duration()).toInt()).toString();
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

        playlist->next();

        emit progressValueChanged(playlist->currentIndex());
        qApp->processEvents();
        if(playlist->currentIndex()==-1){
            emit updated();
            playlist->clear();

            chargeDirectoryWatcher();
        }
    }

}

//--------------------------------------------------------------------
void MediaUpdate::addUpdateDirectory()
{
    QStringList list;
    QSettings settings;
    settings.sync();
    playlist->clear();
    //---------------------------------------
    settings.beginGroup("Options");
     bool remove=settings.value("Clear",false).toBool();
     qDebug()<<"MediaUpdate::addUpdateDirectory::Clear"<<remove;
    if(remove){
      if(DataBase::clearDatabase())
        emit updated();
    }
    settings.setValue("Clear",false);
    settings.endGroup();
    //---------------------------------------

    int count = settings.beginReadArray("Directory");
    if(count==0){ list.append(D_DMUSIC);  }
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        QString dir =settings.value("Dir").toString();
        bool checked=settings.value("Checked").toBool();

        if(checked)  list.append(dir);

    }

    settings.endArray();

    // Scan directory
    // and add media to plylist.
    foreach (QString path, list) {scanDirectory(path);}

    // after scanning if mediacount >0 then
    // start extract metadata and adding to database
    if(playlist->mediaCount()>0){
        emit  progressMaxChanged(playlist->mediaCount());
        playlist->setCurrentIndex(0);
    }
}

//--------------------------------------------------------------------
void MediaUpdate::scanDirectory(const QString &path)
{
    //  مسح المحلدات والمجلدات الفرعية لجلب ملفات
    QDirIterator it(path,supportedMimeTypes(),QDir::Files| QDir::NoDotAndDotDot|QDir::NoSymLinks,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString file= it.next();

        QUrl url(QUrl::fromLocalFile(file));

        playlist->addMedia(url);
    }

}

//--------------------------------------------------------------------
void MediaUpdate::updateDirectory(const QString &dir)
{

    playlist->clear();

    QDirIterator it(dir,supportedMimeTypes(),QDir::Files| QDir::NoDotAndDotDot |QDir::NoSymLinks);
    while (it.hasNext()) {
        QString file= it.next();

        QUrl url(QUrl::fromLocalFile(file));
        playlist->addMedia(url);
    }

    if(playlist->mediaCount()>0){
        emit  progressMaxChanged(playlist->mediaCount());
        playlist->setCurrentIndex(0);
    }

}

//--------------------------------------------------------------------
// add new files to database
void MediaUpdate::addFiles(const QList<QUrl>urls)
{

    playlist->clear();
    for (auto &url: urls) {
      //  qDebug()<<"MediaUpdate adding"<<url;
        playlist->addMedia(url);
    }

    if(playlist->mediaCount()>0)
        playlist->setCurrentIndex(0);
}

// ******************** THREAD **************************
//--------------------------------------------------------------------THREAD
void MediaUpdate::removelast(const QString &key){
//    if(listMap.count()>0) listMap.removeLast();
//    if(listThread.count()>0) listThread.removeLast();
    mMapTread.remove(key);
}

//--------------------------------------------------------------------THREAD
void  MediaUpdate::updateFile(QVariantMap map,const QString &path)
{
//    if(listThread.contains(path))
//        return;
    if(mMapTread.contains(path))return;

    mMapTread.insert(path,map);
    //    listThread.append(path);
    //    listMap.append(map);
    startNewThread();
}



//--------------------------------------------------------------------THREAD
void MediaUpdate::startNewThread()
{
    if(mThread->isRunning())return;

    if(mMapTread.isEmpty())  return;

   // if(listMap.count()>0){
        //QVariantMap map=listMap.last();

    QVariantMap map=mMapTread.last();
        mThread->setFile(map);
        mThread->start();
   // }


    //  emit progressValueChanged(mValue++);
}

//--------------------------------------------------------------------THREAD
Thread:: Thread()
{

    //   mDataBase=data;

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
    //                            mMap.value("icon")  ,
    //                            mMap.value("favo")  ,
    //                            mMap.value("tags")) ;

    emit removeKey(path);
   // emit removelast();
}
