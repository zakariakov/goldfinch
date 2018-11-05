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

#ifndef DATABASE_H
#define DATABASE_H
#include "defines.h"
#include <QObject>
#include <QDir>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QUrl>

#define DATA_NOEXIST    0
#define DATA_EXIST      1
#define DATA_NEEDUPDATE  2

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);

    static DataBase *instance();
    //! فتح قاعدة البيانات
    static void openDataBase();


    //! تحمبل المسارات المقطوعات
    static QList<QVariantMap> chargeAudios(QString name,int colm,
                                           QString pName=QString(),int pColm=0,
                                           QString pPname=QString(),int pPcolm=0);

    //! ارجاع قائمة بالمسارات وعناوين المسارات ان وجدت
    static QList<QVariantMap> getAlbumUrls(QString name, int colm,
                                           QString pName, int pColm,
                                           QString pPname,int pPcolm);


   static QString duration( const QString &path);
   static void setDuration(const QString &duration, const QString &path);

   static  QList<QVariantMap> searchAudios(int col ,const QString &text);
signals:

public slots:

    static  bool fileExist(const QString &path);
    //! حذف كل البيانات
    static  bool clearDatabase();

    //! اضافة مسار جديد لحغظه في قاعدة البيانلت
    static void  addNewSong(const QString &title,const QString &artist,
                            const QString &album,const QString &genre,
                            const QString &path, const QString &duration);
    //! حذف مسار من البيانلت
    static bool removeSong(const QString &path);


    //    void updateExistingSong(const QString &title, const QString &artist,
    //                        const QString &album,   const QString &genre,
    //                        const QString &path, const QString &duration);


    //! اضافة اةازالة من المفضلة
    static bool setFavorite(const QString &path,bool value);

    //! تحميل المسار الوحيد
    static  QStringList chargeRoot(int colm);
    //! تحميل الابناء
    static QStringList chargeChild(int colm,int pColm,
                                   const QString &pName,int pPColm=99,
                                   const QString &pPName=QString());
    //! تحميل المفضلة
    static QStringList  chargeFavoritedAlbum();

private:
    //!
    QSqlDatabase  db;

private slots:
    //! النحقق من البيانات
    static int  checkSongInfo(const QString &path);

    //! تحديث بيانات
    static  bool updateSong(const QString &title,const QString &artist,
                            const QString &album,const QString &genre,
                            const QString &path, const QString &duration);

};

#endif // DATABASE_H
