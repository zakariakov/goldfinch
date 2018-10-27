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

    //! فتح قاعدة البيانات
    void openDataBase();


    //! تحمبل المسارات المقطوعات
    QList<QVariantMap> chargeAudios(QString name,
                                              int colm,
                                              QString pName=QString(),
                                              int pColm=0,
                                              QString pPname=QString(),
                                              int pPcolm=0);
    //! ارجاع قائمة بالمسارات وعناوين المسارات ان وجدت
    QList<QVariantMap> getAlbumUrls(QString name,
                                              int colm,
                                              QString pName,
                                              int pColm,
                                              QString pPname,
                                              int pPcolm);

signals:

public slots:
    //! اضافة مسار جديد لحغظه في قاعدة البيانلت
    void  addNewSong(const QString &title,   const QString &artist,
                           const QString &album,   const QString &genre,
                           const QString &path, const QString &duration);

    void updateExistingSong(const QString &title, const QString &artist,
                        const QString &album,   const QString &genre,
                        const QString &path, const QString &duration);

    bool updateSong(const QString &title,   const QString &artist,
                   const QString &album,   const QString &genre,
                    const QString &path, const QString &duration);

    bool setFavorite(const QString &path,bool value);

    //! تحميل المسار الوحيد
    QStringList chargeRoot(int colm);
    //! تحميل الابناء
    QStringList chargeChild(int colm,int pColm,QString pName,
                            int pPColm=99,QString pPName=QString());


    QStringList  chargeFavoritedAlbum();

private:
    //!
     QSqlDatabase  db;

private slots:
    //!
  //  int idFromFile(const QString &file);
    int  checkSongInfo(const QString &title,   const QString &artist,
                            const QString &album,   const QString &genre,
                            const QString &path, const QString &duration);

};

#endif // DATABASE_H
