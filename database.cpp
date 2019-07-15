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

#include "database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QDebug>
#include <QDateTime>

Q_GLOBAL_STATIC(DataBase, DataBaseInstance)
DataBase *DataBase::instance()
{
    return DataBaseInstance();
}
DataBase::DataBase(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

}

QString colString(int colm)
{
    QString column;
    switch (colm)
    {
    case COL_I_GENRE: column=COL_S_GENRE; break;
    case COL_I_ARTIST:column=COL_S_ARTIST; break;
    case COL_I_ALBUM: column=COL_S_ALBUM;  break;
    case COL_I_RATED: column=COL_S_RATED;  break;
    case COL_I_TITLE: column=COL_S_TITLE;  break;

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
    dir.mkpath(CACHE);

   instance()-> db.setDatabaseName(CACHE+"/library.db");

    QString txt="no database: ";
    if(! instance()-> db.open())
    {
        txt+= instance()-> db.lastError().text();
        qDebug()<<"MyModel::openDataBase()"<<txt;
    }

    //! [Set up table  book]

    QSqlQuery query( instance()-> db);

    //AUTOINCREMENT
    query.exec("CREATE TABLE 'main'.'books' ("
               "'id' INTEGER PRIMARY KEY  NOT NULL,"
               "'title' TEXT,"
               "'artist' TEXT,"
               "'album' TEXT,"
               "'genre' TEXT,"
               "'path' TEXT,"
               "'duration' TEXT,"
               "'favo' INTEGER NOT NULL DEFAULT (0),"
               "'modified' TEXT"
               ");");

     query.exec("CREATE UNIQUE INDEX `idx` ON `books` ( `path`	ASC)");

     query.exec("CREATE TABLE 'main'.'albums' ("
                "'title'  PRIMARY KEY  NOT NULL,"
                "'path' TEXT"
                ");");
    //  query.exec("CREATE UNIQUE INDEX `index_nom` ON `table` (`colonne1`)");
}

bool  DataBase::clearDatabase()
{
     QSqlQuery query(instance()->db);
    QString  txt="DROP TABLE books";
     QString  txt2="DROP TABLE albums";
    if(query.exec(txt)){
        query.exec(txt2);
        openDataBase();
        return true;
    } else{
        qDebug()<<"DataBase::clearDatabase::"<<query.lastError().text();
    }


    return false;
}

QString DataBase::duration( const QString &path)
{
    QString   txt=QString("select duration from books WHERE path='%1'")
            .arg(inTxt(path));
    QSqlQuery query(instance()->db);
    query.exec(txt);
    while(query.next())
    {
        return query.value(0).toString();
    }
    return QString();
}

void  DataBase::setDuration(const QString &duration, const QString &path)
{
    QString txt=QString("UPDATE books "
                        " SET duration = '%1'"
                        " WHERE path= '%2'").arg(duration).arg(inTxt(path));

    QSqlQuery query(instance()->db);
    // qDebug()<<txt;
    if(query.exec(txt))
        qDebug()<<"duration Updated";
    else{
        qDebug()<<"setDuration ::"<< query.lastError().text();
        qDebug()<<"setDuration ::" <<txt;
    }
}

bool DataBase::fileExist(const QString &path)
{
    QFileInfo info(path);
    QString lastModified= info.lastModified().toString("dd MM yyyy hh:mm:ss");

    QString   txt=QString("select path,modified from books WHERE path='%1'")
            .arg(inTxt(path));
    QSqlQuery query(instance()->db);
    query.exec(txt);

    while(query.next())
    {
        qDebug()<<"data"<<query.value(1).toString()<<query.value(0).toString();
        if(lastModified!=query.value(1).toString())
            return false;

        return true;
    }

    return false;
}
//_____________________________________________________________________
//_____________________________________________________________________
int  DataBase::checkSongInfo( const QString &path)

{
    QFileInfo info(path);
    QString lastModified= info.lastModified().toString("dd MM yyyy hh:mm:ss");

    QString   txt=QString("select path,modified from books WHERE path='%1'")
            .arg(inTxt(path));
    QSqlQuery query(instance()->db);
    query.exec(txt);

    while(query.next())
    {
        qDebug()<<"DataBase::checkSongInfo"<<query.value(1).toString()<<query.value(0).toString();
        if(lastModified!=query.value(1).toString())
            return DATA_NEEDUPDATE;

        return DATA_EXIST;
    }

    return DATA_NOEXIST;

}
bool DataBase::removeSong(const QString &path)
{
    //----------------------------------------------------------2
    QString text=QString("DELETE FROM books WHERE path='%1'")
            .arg(inTxt(path));
 QSqlQuery query(instance()->db);
    if(query.exec(text))
        return true;
    else
        qDebug()<<"DataBase::removeSong"<<query.lastError().text();
    return false;
}
//_____________________________________________________________________
void  DataBase::addNewSong(const QString &title,   const QString &artist,
                           const QString &album,   const QString &genre,
                           const QString &path, const QString &duration)
{

    if(path.isEmpty()) return;

    int ret=checkSongInfo( path);

    switch (ret) {
    case DATA_NOEXIST:
        //  qDebug ()<<"DATA_NOEXIST: "<<title;
        break;
    case DATA_EXIST:
        qDebug ()<<"DATA_EXIST: "<<title;
        return;
    case DATA_NEEDUPDATE:
        qDebug ()<<"DATA_NIDUPDATE: "<<title;
        updateSong(title,artist,album,genre,path,duration);
        return;
    default:
        break;
    }


    QSqlQuery query(instance()->db);

    /******************************************************************
   *
   * insert columns into Books in order
   * id=0 1=title 2=artist  3=album 4=genre  5=path 6=duration 7=favo
   *
   * ***************************************************************/
    QFileInfo info(path);
    QString lastModified= info.lastModified().toString("dd MM yyyy hh:mm:ss");
    QString txt=QString("insert into books values(NULL,'%1','%2','%3','%4','%5','%6','%7','%8')")
            .arg(inTxt(title))
            .arg(inTxt(artist))
            .arg(inTxt(album))
            .arg(inTxt(genre))
            .arg(inTxt(path))
            .arg(inTxt(duration))
            .arg(0)
            .arg(lastModified)
            ;

    if(query.exec(txt)){
        // qDebug()<<"add new song"<<title<<artist<<album<<genre<<path<<duration<<favo<<tags;
    }else{
        qDebug()<<"error adding song"<<title<<artist<<album<<genre<<path<<duration;
        qDebug()<< query.lastError().text();
    }
}

 void  DataBase::addNewAlbumImgPath(const QString &title,const QString &path)
 {
     QSqlQuery query(instance()->db);
     QString txt=QString("insert into albums values('%1','%2')")
             .arg(inTxt(title))
             .arg(inTxt(path))
             ;



     if(query.exec(txt)){
         // qDebug()<<"add new song"<<title<<artist<<album<<genre<<path<<duration<<favo<<tags;
     }else{
         QString txt=QString("UPDATE albums "
                             " SET %1 ='%2' "
                             " WHERE title='%3' ")
                 .arg(COL_S_PATH).arg(inTxt(path)).arg(inTxt(title));
         query.exec(txt);
         qDebug()<<"addNewAlbumImgPath"<< txt;
         qDebug()<<"addNewAlbumImgPath"<< query.lastError().text();
     }

 }

 QString  DataBase::getAlbumImgPath(const QString &title)
 {
     QString   txt=QString("select path from albums WHERE title='%1'")
             .arg(inTxt(title));
     QSqlQuery query(instance()->db);
     query.exec(txt);
     while(query.next())
     {
         QString txt=outTxt(query.value(0).toString());
         return txt;
     }
     return QString();
 }

bool DataBase::updateSong(const QString &title,   const QString &artist,
                          const QString &album,   const QString &genre,
                          const QString &path , const QString &duration)
{
    QFileInfo info(path);
    QString lastModified= info.lastModified().toString("dd MM yyyy hh:mm:ss");


    QString txt=QString("UPDATE books"
                        " SET %1 = '%2',"
                        "  %3 = '%4',"
                        "  %5 = '%6',"
                        "  %7 = '%8' ,"
                        "  %9 = '%10' , "
                        "  %11 = '%12' "
                        " WHERE path= '%13'")
            .arg(COL_S_TITLE).arg(inTxt(title))  /*1=2*/
            .arg(COL_S_ARTIST).arg(inTxt(artist)) /*3=4*/
            .arg(COL_S_ALBUM).arg(inTxt(album)) /*5=6*/
            .arg(COL_S_GENRE).arg(inTxt(genre)) /*7=8*/
            .arg(COL_S_DURATION).arg(duration.trimmed()) /*9=10*/
            .arg(COL_S_MODIF).arg(lastModified) /*11=12*/

            .arg(inTxt(path)) /*13*/;

    QSqlQuery query(instance()->db);
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
            .arg(inTxt(path)); /*3=4*/    // qDebug()<<txt;
    QSqlQuery query(instance()->db);
    if(query.exec(txt))
        return true;

    qDebug()<< query.lastError().text();
    qDebug()<<txt;

    return false;
}

//----------------------------------------------------------Chargements

QStringList  DataBase::chargeRoot(int colm)
{
    QSqlQuery query(instance()->db);

    QString column=colString(colm);


    query.exec(QString("SELECT DISTINCT %1 FROM books  WHERE path IS NOT NULL").arg(column));
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
    QSqlQuery query(instance()->db);



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
                                   int pColm,const QString &pName,
                                   int pPColm,const QString &pPName)
{

    QString column=colString(colm);
    QString pColumn=colString(pColm);
    QString pPColumn=colString(pPColm);

    QString txt;
    if(pPColumn.isEmpty()|| pPName.isEmpty()){
        txt=QString("select DISTINCT %1 from books WHERE %2='%3'  AND path IS NOT NULL").arg(column)
                .arg(pColumn).arg(inTxt(pName));

    }else{
        txt=QString("select DISTINCT %1 from books WHERE %2='%3' AND %4='%5'  AND path IS NOT NULL").arg(column)
                .arg(pColumn).arg(inTxt(pName)).arg(pPColumn).arg(inTxt(pPName));

    }
    //qDebug()<<"query"<<txt;
    QSqlQuery query(instance()->db);
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
        txt=QString("select * from books WHERE %1='%2'  AND path IS NOT NULL" )
                .arg(colm_s).arg(inTxt(name));
    else if (pPname.isEmpty())
        txt=QString("select * from books WHERE %1='%2' AND %3='%4'  AND path IS NOT NULL")
                .arg(colm_s).arg(inTxt(name)).arg(pColm_s).arg(inTxt(pName));
    else
        txt=QString("select * from books WHERE %1='%2' AND %3='%4' AND %5='%6' AND path IS NOT NULL")
                .arg(colm_s).arg(inTxt(name)).arg(pColm_s).arg(inTxt(pName)).arg(pPColm_s).arg(inTxt(pPname));



    QSqlQuery query(instance()->db);
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

        map[COL_S_TITLE] =query.value(COL_I_TITLE).toString().replace("$","'");
        map[COL_S_ARTIST]=query.value(COL_I_ARTIST).toString().replace("$","'");
        map[COL_S_ALBUM] =query.value(COL_I_ALBUM).toString().replace("$","'");
        map[COL_S_GENRE] =query.value(COL_I_GENRE).toString().replace("$","'");
        map[COL_S_PATH]  =query.value(COL_I_PATH).toString().replace("$","'");
        map[COL_S_DURATION]  =query.value(COL_I_DURATION).toString();
        map[COL_S_RATED] =query.value(COL_I_RATED).toString();
        //map[COLM_TIME]  =query.value(CAT_TIME).toString();
       // qDebug()<<query.value(COL_I_TITLE).toString()<<query.value(COL_I_DURATION).toString();
        list.append(map);

    }
    return list;

}

 QList<QVariantMap> DataBase::searchAudios(int col ,const QString &text)
{
    QString colm_s=colString(col);
 //   qDebug()<<col<<colm_s<<text;
    QString  txt=QString("select * from books WHERE %1 LIKE '%%2%'" )
            .arg(colm_s,text);
//qDebug()<<txt;
    QSqlQuery query(instance()->db);
    if(query.exec(txt))
     qDebug()<< "QSqlQuery Audio:"<<txt;
    else
         qDebug()<< "QSqlQuery Audio:"<<query.lastError().text();

    QList<QVariantMap> list;
    QVariantMap map;
    while(query.next())
    {
        /**********************************************************************
         * id=0 1=title 2=artist  3=album 4=genre  5=path 6=icon 7=favo 8=tags
         **********************************************************************/

        QString path=query.value(5).toString();
        if(path.isEmpty()||path.isNull())
            continue;

        map[COL_S_TITLE] =query.value(COL_I_TITLE).toString().replace("$","'");
        map[COL_S_ARTIST]=query.value(COL_I_ARTIST).toString().replace("$","'");
        map[COL_S_ALBUM] =query.value(COL_I_ALBUM).toString().replace("$","'");
        map[COL_S_GENRE] =query.value(COL_I_GENRE).toString().replace("$","'");
        map[COL_S_PATH]  =query.value(COL_I_PATH).toString().replace("$","'");
        map[COL_S_DURATION]  =query.value(COL_I_DURATION).toString();
        map[COL_S_RATED] =query.value(COL_I_RATED).toString();
        //map[COLM_TIME]  =query.value(CAT_TIME).toString();
     //   qDebug()<<query.value(COL_I_TITLE).toString()<<query.value(COL_I_DURATION).toString();
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



    QSqlQuery query(instance()->db);
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


 QVariantMap DataBase::filePropery(const QString & file)
 {

     QVariantMap map;

     QString   txt=QString("select * from books WHERE path='%1'")
             .arg(inTxt(file));
     QSqlQuery query(instance()->db);
     query.exec(txt);

     while(query.next())
     {
         map[COL_S_TITLE] =query.value(COL_I_TITLE).toString().replace("$","'");
         map[COL_S_ARTIST]=query.value(COL_I_ARTIST).toString().replace("$","'");
         map[COL_S_ALBUM] =query.value(COL_I_ALBUM).toString().replace("$","'");
         map[COL_S_GENRE] =query.value(COL_I_GENRE).toString().replace("$","'");
         map[COL_S_PATH]  =query.value(COL_I_PATH).toString().replace("$","'");
         map[COL_S_DURATION]  =query.value(COL_I_DURATION).toString();

     }

     return map;
 }

  int DataBase::removeNonExisttingFiles()
  {
int n=0;
      QString   txt=QString("select path from books");

      QSqlQuery query(instance()->db);
      query.exec(txt);

      while(query.next())
      {

         QString path =query.value(0).toString();
         // qDebug()<<"removeNonExisttingFiles"<<path;
         QString file= path.replace("$","'");
         if(!QFile::exists(file)){
             n++;
            removeSong(path);
             qDebug()<<"Non Exist"<<file;
         }

      }
      return n;
  }
