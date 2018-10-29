#ifndef MEDIAUPDATE_H
#define MEDIAUPDATE_H
#include "database.h"
#include <QObject>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QThread>
#include <QFileSystemWatcher>
//*********************THREAD**************************
class Thread : public QThread
{
    Q_OBJECT

public:
    Thread();
    void setFile(QVariantMap map){mMap=map;}
    QUrl curentPath(){return mUrl;}
signals:
    void removelast();
    void removeKey(const QString &key);
protected:
    void run();

private:
    QUrl mUrl;
    QVariantMap mMap;
    DataBase *mDataBase;


private slots:

};

//*********************MediaUpdate**************************
class MediaUpdate : public QObject
{
    Q_OBJECT
public:
    explicit MediaUpdate(QObject *parent = nullptr);
  ~MediaUpdate();
signals:
    void updated();
    void progressMaxChanged(int max);
    void progressValueChanged(int value);
    void dirNidUpdate(bool);
public slots:
    void addUpdateDirectory();
    void scanDirectory(const QString &path);
    void updateDirectory(const QString &dir);
    void addFiles(const QList<QUrl>urls);
    void updateFile(QVariantMap map, const QString &path);
void setUpdateDirs(bool update);
//    void setlistDirs(QStringList list){mLisDirs=list;}
//    QStringList listDirs(){return mLisDirs;}

private slots:
    void metaDataChanged();
    void chargeDirectoryWatcher();
void directoryChanged(const QString &path);

    //thread
    void startNewThread();
    void removelast(const QString &key);
private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
  //  DataBase *mDataBase;
    Thread   *mThread;
    QList<QUrl> mListMedia;

   QStringList mLisUpdateDirs;
QFileSystemWatcher *mWatcher;
    //thred
QList <QVariantMap>listMap;

QStringList listThread;
QMap<QString,QVariantMap> mMapTread;
    int mValue;


};

#endif // MEDIAUPDATE_H
