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

#include "setting.h"
#include "defines.h"
#include <QDir>
#include <QDebug>
#include <QUrl>
Setting::Setting(QObject *parent ):QSettings(parent)
{
}

void Setting::setAlbumFavo(const QString &title,bool favo)
{

    //QSettings s(CACHE+"/albums",QSettings::IniFormat);
    QSettings s(APP_NAME,"albums");
    s.beginGroup("Favorite");

    if(favo)
        s. setValue(title,1);
    else{
        if(s.contains(title))
            s.remove(title);
    }

    s.endGroup();
}

bool Setting::albumIsFavorited(const QString &title)
{
  //  QSettings s(CACHE+"/",QSettings::IniFormat);
    QSettings s(APP_NAME,"albums");
    s.beginGroup("Favorite");
    bool favo=s.value(title,0).toBool();
    s.endGroup();
    return favo;
}

QStringList  Setting::favoretedAlbum()
{
  //  QSettings s(CACHE+"/albums",QSettings::IniFormat);
        QSettings s(APP_NAME,"albums");
    QStringList list;
    s.beginGroup("Favorite");
    list= s.allKeys();
    s.endGroup();
    return list;
}

QString Setting::albumImgPath(const QString &title)
{
  QSettings s(CACHE+"/albums",QSettings::IniFormat);
s.setIniCodec("UTF_8");
     //   qDebug()<<"albumImgPath"<<title<<s.value(title).toString().replace("%"," ");

    return s.value(title).toString();

}

void Setting::setAlbumImgPath(const QString &album,const QFileInfo &file)
   {
    QSettings s(CACHE+"/albums",QSettings::IniFormat);
    s.setIniCodec("UTF_8");

    s.setValue( album,file.absolutePath());
   }


 void  Setting::saveRecent(QVariantMap map, int index)
 {
     QSettings settings;
     settings.beginGroup("Recent");

     settings.setValue("Index", index);
     settings.setValue("Map",  map);

     settings.endGroup();
 }

   QVariantMap  Setting:: getRecentMap()
   {
       QSettings settings;
       settings.beginGroup("Recent");
       return  settings.value("Map").toMap();

   }
    int  Setting:: getRecentndex()
    {
        QSettings settings;
        settings.beginGroup("Recent");
        return  settings.value("Index").toInt();

    }
