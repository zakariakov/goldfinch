#include "database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

QString colString(int colm)
{
    QString column;
    switch (colm)
    {
    case CAT_GENRE: column=COLM_GENRE; break;
    case CAT_ARTIST:column=COLM_ARTIST; break;
    case CAT_ALBUM: column=COLM_ALBUM;  break;
    case CAT_RATED: column=COLM_RATED;  break;
    default:  break;

    }
    return  column;
}
QString inTxt(QString txt){  return txt.trimmed().replace("'","$");}
QString outTxt(QString txt){  return txt.trimmed().replace("$","'");}


//_____________________________________________________
void DataBase::openDataBase()
{
    QDir dir;
    dir.mkpath(D_CACHE);

    db.setDatabaseName(D_CACHE+"/library.db");

    QString txt="no database: ";
    if(!db.open())
    {
        txt+=db.lastError().text();
        qDebug()<<"MyModel::openDataBase()"<<txt;
    }

    //! [Set up table  book]

    QSqlQuery query(db);

    //AUTOINCREMENT
    query.exec("CREATE TABLE 'main'.'books' ("
               "'id' INTEGER PRIMARY KEY  NOT NULL,"
               "'title' TEXT,"
               "'artist' TEXT,"
               "'album' TEXT,"
               "'genre' TEXT,"
               "'path' TEXT,"
               "'duration' TEXT,"
               "'favo' INTEGER NOT NULL DEFAULT (0)"
               ");");

}
//_____________________________________________________________________
void  DataBase::updateExistingSong(const QString &title,
                                   const QString &artist,
                                   const QString &album,
                                   const QString &genre,
                                   const QString &path,
                                   const QString &duration)
{
    int ret=checkSongInfo(title,artist, album,genre, path,duration);

    switch (ret) {
    case DATA_NOEXIST:
        qDebug ()<<"updateExistingSong NOEXIST: "<<path;
        return ;
    case DATA_EXIST:
        qDebug ()<<"updateExistingSong EXIST: "<<title;;
        return;
    case DATA_NIDUPDATE:
        qDebug ()<<"updateExistingSong NIDUPDATE: "<<title<<duration;;
        updateSong(title,artist,album,genre,path,duration);
        return;
    default:
        break;
    }



}

//_____________________________________________________________________
int  DataBase::checkSongInfo(const QString &title,   const QString &artist,
                             const QString &album,   const QString &genre,
                             const QString &path, const QString &duration)
{

    QString   txt=QString("select * from books WHERE path='%1'")
            .arg(inTxt(path));
    QSqlQuery query(db);
    query.exec(txt);
    while(query.next())
    {
        QString _title= query.value(CAT_TITLE).toString();
        if(!title.isEmpty() &&_title!=title)    return DATA_NIDUPDATE;

        QString _artist= query.value(CAT_ARTIST).toString();
        if(!artist.isEmpty() &&_artist!=artist) return DATA_NIDUPDATE;

        QString _album= query.value(CAT_ALBUM).toString();
        if(!album.isEmpty() &&_album!=album)    return DATA_NIDUPDATE;

        QString _genre= query.value(CAT_GENRE).toString();
        if(!genre.isEmpty() &&_genre!=genre)    return DATA_NIDUPDATE;

        QString _duration= query.value(CAT_DURATION).toString();
          qDebug()<<"DATA duration"<<_duration<<duration;
        if(!duration.isEmpty() &&_duration!=duration){

            return DATA_NIDUPDATE;
        }

        return DATA_EXIST;
    }

    return DATA_NOEXIST;
}

//_____________________________________________________________________
void  DataBase::addNewSong(const QString &title,   const QString &artist,
                           const QString &album,   const QString &genre,
                           const QString &path, const QString &duration)
{

    int ret=checkSongInfo(title,artist,album, genre, path,duration);

    switch (ret) {
    case DATA_NOEXIST:
        //  qDebug ()<<"DATA_NOEXIST: "<<title;
        break;
    case DATA_EXIST:
        qDebug ()<<"DATA_EXIST: "<<title;;
        return;
    case DATA_NIDUPDATE:
        qDebug ()<<"DATA_NIDUPDATE: "<<title;;
        updateSong(title,artist,album,genre,path,duration);
        return;
    default:
        break;
    }


    QSqlQuery query(db);

    /******************************************************************
   *
   * insert columns into Books in order
   * id=0 1=title 2=artist  3=album 4=genre  5=path 6=duration 7=favo
   *
   * ***************************************************************/

    QString txt=QString("insert into books values(NULL,'%1','%2','%3','%4','%5','%6','%7')")
            .arg(inTxt(title))
            .arg(inTxt(artist))
            .arg(inTxt(album))
            .arg(inTxt(genre))
            .arg(inTxt(path))
            .arg(inTxt(duration))
            .arg(0)
            ;

    if(query.exec(txt)){
        // qDebug()<<"add new song"<<title<<artist<<album<<genre<<path<<duration<<favo<<tags;
    }else{
        qDebug()<<"error adding song"<<title<<artist<<album<<genre<<path<<duration;
        qDebug()<< query.lastError().text();
    }
}

bool DataBase::updateSong(const QString &title,   const QString &artist,
                          const QString &album,   const QString &genre,
                          const QString &path , const QString &duration)
{


    QString txt=QString("UPDATE books"
                        " SET %1 = '%2',"
                        "  %3 = '%4',"
                        "  %5 = '%6',"
                        "  %7 = '%8' ,"
                        "  %9 = '%10' "
                        " WHERE path= '%11'")
            .arg(COLM_TITLE).arg(inTxt(title))  /*1=2*/
            .arg(COLM_ARTIST).arg(inTxt(artist)) /*3=4*/
            .arg(COLM_ALBUM).arg(inTxt(album)) /*5=6*/
            .arg(COLM_GENRE).arg(inTxt(genre)) /*7=8*/
            .arg(COLM_DURATION).arg(duration.trimmed()) /*9=10*/
            .arg(inTxt(path)) /*11*/;

    QSqlQuery query(db);
    // qDebug()<<txt;
    if(query.exec(txt))
        return true;
    else{
        qDebug()<< query.lastError().text();
        qDebug()<<txt;
    }
    return false;
}



bool DataBase::setFavorite(const QString &path,bool value)
{
    QString txt=QString("UPDATE books"
                        " SET favo = %1 "
                        " WHERE path= '%2'")
            .arg(QString::number(value))  /*1=2*/
            .arg(inTxt(path)); /*3=4*/;
    // qDebug()<<txt;
    QSqlQuery query(db);
    if(query.exec(txt))
        return true;

    return false;
}

//----------------------------------------------------------Chargements

QStringList  DataBase::chargeRoot(int colm)
{
    QSqlQuery query;

    QString column=colString(colm);


    query.exec(QString("SELECT DISTINCT %1 FROM books").arg(column));
    QStringList list;
    while(query.next())
    {
        QString name=query.value(0).toString();
        if(name.isEmpty()||name.isNull())
            continue;
        list.append(outTxt(name));

    }

    return list;

}

QStringList  DataBase::chargeFavoritedAlbum()
{
    QSqlQuery query;



    query.exec(QString("SELECT DISTINCT album FROM books WHERE favo='1'"));
    QStringList list;
    while(query.next())
    {
        QString name=query.value(0).toString();
        if(name.isEmpty()||name.isNull())
            continue;
        list.append(outTxt(name));

    }

    return list;
}
//-----------------------------------------------------------
QStringList  DataBase::chargeChild(int colm,
                                   int pColm,QString pName,
                                   int pPColm,QString pPName)
{

    QString column=colString(colm);
    QString pColumn=colString(pColm);
    QString pPColumn=colString(pPColm);

    QString txt;
    if(pPColumn.isEmpty()|| pPName.isEmpty()){
        txt=QString("select DISTINCT %1 from books WHERE %2='%3'").arg(column)
                .arg(pColumn).arg(inTxt(pName));

    }else{
        txt=QString("select DISTINCT %1 from books WHERE %2='%3' AND %4='%5'").arg(column)
                .arg(pColumn).arg(inTxt(pName)).arg(pPColumn).arg(inTxt(pPName));

    }
    //qDebug()<<"query"<<txt;
    QSqlQuery query;
    query.exec(txt);

    QStringList list;
    while(query.next())
    {
        QString name=query.value(0).toString();
        if(name.isEmpty()||name.isNull())
            continue;
        list.append(outTxt(name));

    }

    return list;

}

QList<QVariantMap> DataBase::chargeAudios(QString name,
                                          int colm,
                                          QString pName,
                                          int pColm,
                                          QString pPname,
                                          int pPcolm)
{

    QString colm_s=colString(colm);
    QString pColm_s=colString(pColm);
    QString pPColm_s=colString(pPcolm);

    // QStringList list;
    QList<QVariantMap> list;
    QVariantMap map;
    QString   txt;


    if(pName.isEmpty())
        txt=QString("select * from books WHERE %1='%2'" )
                .arg(colm_s).arg(inTxt(name));
    else if (pPname.isEmpty())
        txt=QString("select * from books WHERE %1='%2' AND %3='%4'")
                .arg(colm_s).arg(inTxt(name)).arg(pColm_s).arg(inTxt(pName));
    else
        txt=QString("select * from books WHERE %1='%2' AND %3='%4' AND %5='%6'")
                .arg(colm_s).arg(inTxt(name)).arg(pColm_s).arg(inTxt(pName)).arg(pPColm_s).arg(inTxt(pPname));



    QSqlQuery query;
    query.exec(txt);
    //   qDebug()<< "QSqlQuery Audio:"<<txt;

    while(query.next())
    {
        /**********************************************************************
          * id=0 1=title 2=artist  3=album 4=genre  5=path 6=icon 7=favo 8=tags
          **********************************************************************/

        QString path=query.value(5).toString();
        if(path.isEmpty()||path.isNull())
            continue;

        map[COLM_TITLE] =query.value(CAT_TITLE).toString().replace("$","'");
        map[COLM_ARTIST]=query.value(CAT_ARTIST).toString().replace("$","'");
        map[COLM_ALBUM] =query.value(CAT_ALBUM).toString().replace("$","'");
        map[COLM_GENRE] =query.value(CAT_GENRE).toString().replace("$","'");
        map[COLM_PATH]  =query.value(CAT_PATH).toString().replace("$","'");
        map[COLM_DURATION]  =query.value(CAT_DURATION).toString();
        map[COLM_RATED] =query.value(CAT_RATED).toString();
        //map[COLM_TIME]  =query.value(CAT_TIME).toString();
        qDebug()<<query.value(CAT_TITLE).toString()<<query.value(CAT_DURATION).toString();
        list.append(map);

    }
    return list;

}

QList<QVariantMap> DataBase::getAlbumUrls(QString name,
                                          int colm,
                                          QString pName,
                                          int pColm,
                                          QString pPname,
                                          int pPcolm)
{


    QString colm_s=colString(colm);
    QString pColm_s=colString(pColm);
    QString pPColm_s=colString(pPcolm);

    // QStringList list;
    //   QList<QUrl> list;
    QList<QVariantMap> list;


    QString   txt;


    if(pName.isEmpty())
        txt=QString("select  path,title from books  WHERE %1='%2' ORDER BY title" )
                .arg(colm_s).arg(inTxt(name));
    else if (pPname.isEmpty())
        txt=QString("select  path,title from books WHERE %1='%2' AND %3='%4' ORDER BY title")
                .arg(colm_s).arg(inTxt(name)).arg(pColm_s).arg(inTxt(pName));
    else
        txt=QString("select path,title from books WHERE %1='%2' AND %3='%4' AND %5='%6' ORDER BY title")
                .arg(colm_s).arg(inTxt(name)).arg(pColm_s).arg(inTxt(pName)).arg(pPColm_s).arg(inTxt(pPname));



    QSqlQuery query;
    query.exec(txt);
    //  qDebug()<< "QSqlQuery Audio:"<<txt;

    while(query.next())
    {
        QString str=query.value(0).toString();
        QString tit=query.value(1).toString();
        QMap<QString,QVariant> map;
        map["Title"]=outTxt(tit);
        map["Url"]=outTxt(str);


        list.append(map);
    }

    return list;
}

