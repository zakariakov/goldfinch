#include "tagid.h"
#include "defines.h"
#ifdef USE_LIB_TAG
#include "taglib/fileref.h"
#include "taglib/tag.h"

#include <taglib/tfile.h>
#include <taglib/fileref.h>
#endif
#include <QDebug>
#include <QFileInfo>
Q_GLOBAL_STATIC(TagId, TagIdInstance)
TagId *TagId::instance()
{
    return TagIdInstance();
}

TagId::TagId(QObject *parent) : QObject(parent)
{

}

QVariantMap TagId::mediaTags(const QString &file)
{
    QVariantMap map;
    qDebug()<<file;
    QFileInfo fi(file);

    map[COL_S_PATH]=file;

#ifdef USE_LIB_TAG
    TagLib::FileRef f(file.toLocal8Bit());

    if(!f.isNull() && f.tag()) {

        TagLib::Tag *tag = f.tag();

        QString tit=tag->title().toCString(true);
        map[COL_S_TITLE]=tit.isEmpty()?fi.fileName():tit;

        QString artist=tag->artist() .toCString(true);
        map[COL_S_ARTIST]=!artist.isEmpty() ? artist: tr("Unknown");

        QString album=tag->album().toCString(true) ;
        map[COL_S_ALBUM]= !album.isEmpty() ? album: fi.dir().dirName();

        map[COL_S_YEAR]=  tag->year()  ;

        map[COL_S_COMMENT]=  tag->comment().toCString(true) ;

        map[COL_S_TRACK]= tag->track()   ;

        QString genre=tag->genre() .toCString(true) ;
        map[COL_S_GENRE]= !genre.isEmpty() ? genre:tr("Unknown");

    }else{
        map[COL_S_TITLE]=fi.fileName();
        map[COL_S_ARTIST]=tr("Unknown");
        map[COL_S_ALBUM]= fi.dir().dirName();
        map[COL_S_GENRE]= tr("Unknown");

    }

    if(!f.isNull() && f.audioProperties()) {

        TagLib::AudioProperties *properties = f.audioProperties();

        int seconds = properties->length() % 60;
        int hour=  (properties->length()/3600) ;
        int m = (properties->length() - seconds) / 60;
        int minutes=m-(hour*60);

        QString _h=QString::number(hour).rightJustified(2, '0');
        QString _m=QString::number(minutes).rightJustified(2, '0');
        QString _s=QString::number(seconds).rightJustified(2, '0');

        map[COL_S_TIME]=_h+":"+_m+":"+_s;

        map[COL_S_LENGHT]= properties->length();

        map[COL_S_BITRATE]= properties->bitrate() ;

        map[COL_S_SAMPLE_RATE]= properties->sampleRate() ;

        map[COL_S_CHANNELS]=  properties->channels() ;

        //  qDebug() << "length      - " << minutes << ":" << setfill('0') << setw(2) << seconds << endl;
    }else{
         map[COL_S_TIME]="00:00:00";
    }


#endif

    return  map;
}

//------------------------------------------------------------------------------------------------------------

void TagId::setMediaTags( QVariantMap mMap)
{

    QString title=mMap.value(COL_S_TITLE).toString();
    QString album=mMap.value(COL_S_ALBUM).toString();
    QString artist=mMap.value(COL_S_ARTIST).toString();
    QString genre=mMap.value(COL_S_GENRE).toString();
    QString file=mMap.value(COL_S_PATH).toString();

    //    TODO : add comment year track
    //    QString comment=mMap.value(COL_S_COMMENT).toString();
    //    QString year=mMap.value(COL_S_YEAR).toString();
    //    QString track=mMap.value(COL_S_TRACK).toString();

    QFileInfo fn(file);

    if(!fn.isWritable())
        return;

    TagLib::FileRef f(file.toLocal8Bit());

    TagLib::Tag *t= f.tag();
    if(f.isNull() && !f.tag()){
        qDebug()<<"tag is null";
    }

    if(!title.isEmpty())          t->setTitle(title.toStdWString());
    if(!album.isEmpty())      t->setAlbum(album.toStdWString());
    if(!artist.isEmpty())        t->setArtist(artist.toStdWString());
    if(!genre.isEmpty())       t->setGenre(genre.toStdWString());

    if(f.save()){

        qDebug()<<"tag saved:"<<file<<title<<album<<artist<<genre;

       emit instance()->updateFile(file);
    }
}

