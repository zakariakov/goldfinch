#include "setting.h"
#include "defines.h"
#include <QDir>
Setting::Setting(QObject *parent ):
     QSettings(D_CACHE+"/albums",QSettings::IniFormat, parent)
{


}

 void Setting::setAlbumFavo(const QString &title,bool favo)
 {

      QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
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
      QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
     s.beginGroup("Favorite");
     bool favo=s.value(title,0).toBool();
     s.endGroup();
     return favo;
 }

 QStringList  Setting::favoretedAlbum()
 {
        QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
     QStringList list;
     s.beginGroup("Favorite");
     list= s.allKeys();
     s.endGroup();
     return list;
 }

  QString Setting::albumImgPath(const QString &title)
  {
        QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
      return s.value(title).toString();

  }
