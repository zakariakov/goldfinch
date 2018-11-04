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
signals:
    void removeKey(const QString &key);
protected:
    void run();

private:
    QVariantMap mMap;

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
    void directoryNeedUpdate(bool);

public slots:

    //!  اضافة ملفات جديدة
    void addFiles(const QList<QUrl>urls);
    //!  تحديث ملف
    void updateFile(QVariantMap map, const QString &path);
    //!  تحديث مجلدات
    void setUpdateDirs(bool update);
    //!
    void getDirListOptions();

private slots:

    //!
    void metaDataChanged();
    //!  تحميل المجلدات المراقبة
    void chargeDirectoryWatcher();
    //!   اشارة عند نغير مجلد
    void directoryChanged(const QString &path);
    //! تحديث مجلد بعد الطلب
    void updateDirectory(const QString &dir);
    //!  اضافة او تحديث مجلدات
    void addUpdateDirectories(bool all);
    //!  فحص مجلد
    void scanDirectory(const QString &path);

    //!  thread
    void startNewThread();
    //!  thread
    void removelast(const QString &key);

private:
    QMediaPlayer *mPlayer;
    QMediaPlaylist *mPlaylist;
    Thread   *mThread;
    QStringList mLisUpdateDirs;
    QFileSystemWatcher *mWatcher;
    QList <QVariantMap>mListMap;
    QMap<QString,QVariantMap> mMapTread;

};

#endif // MEDIAUPDATE_H
