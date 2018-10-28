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
#define DATA_NIDUPDATE  2

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


signals:

public slots:
    //! حذف كل البيانات
    static  bool clearDatabase();

    //! اضافة مسار جديد لحغظه في قاعدة البيانلت
    static void  addNewSong(const QString &title,const QString &artist,
                            const QString &album,const QString &genre,
                            const QString &path, const QString &duration);
    //! حذف مسار من البيانلت
    static void removeSong(const QString &path);


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
    static int  checkSongInfo(const QString &title,const QString &artist,
                              const QString &album,const QString &genre,
                              const QString &path, const QString &duration);

    //! تحديث بيانات
    static  bool updateSong(const QString &title,const QString &artist,
                            const QString &album,const QString &genre,
                            const QString &path, const QString &duration);

};

#endif // DATABASE_H
