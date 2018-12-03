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

#ifndef MEDIAUPDATE_H
#define MEDIAUPDATE_H
#ifdef USE_LIB_TAG
#include "tagid.h"
#endif
#include "database.h"
#include <QObject>
#include <QWidget>
//#include <QMediaPlayer>
//#include <QMediaPlaylist>
#include <QThread>
#include <QFileSystemWatcher>
//*********************THREAD**************************
class Thread : public QThread
{
    Q_OBJECT

public:
    Thread();
    void setFile(QString file){mFile=file;}
signals:
    void removeKey(const QString &key);
protected:
    void run();

private:

QString mFile;

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
    //void updateFile(QVariantMap map, const QString &path);
   void addupdates(const QString file=QString());
    //!  تحديث مجلدات
    void setUpdateDirs(bool update);
    //!
   // void showSettings();
   void  updateAllDirectories();
   //!  اضافة او تحديث مجلدات
   void addUpdateDirectories(bool all=true);
private slots:


    //!  تحميل المجلدات المراقبة
    void chargeDirectoryWatcher();
    //!   اشارة عند نغير مجلد
    void directoryChanged(const QString &path);
    //! تحديث مجلد بعد الطلب
    void updateDirectory(const QString &dir);

    //!  فحص مجلد
    void scanDirectory(const QString &path);

    //!  thread
    void startNewThread();
    //!  thread
    void removelast(const QString &key);

private:
 //   QMediaPlayer *mPlayer;
 //   QMediaPlaylist *mPlaylist;
    Thread   *mThread;
    QStringList mLisUpdateDirs;

    QStringList listFiles;
    QFileSystemWatcher *mWatcher;



};

#endif // MEDIAUPDATE_H
