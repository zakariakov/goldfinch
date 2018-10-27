#include "setting.h"
#include "defines.h"
#include <QDir>
Setting::Setting(QObject *parent ):
     QSettings(D_CACHE+"/albums",QSettings::IniFormat, parent)
{


}

 void Setting::setAlbumFavo(const QString &title,bool favo)
 {

     beginGroup("Favorite");

     if(favo)
         setValue(title,1);
     else{
         if(contains(title))
             remove(title);
     }

     endGroup();
 }

 bool Setting::albumIsFavorited(const QString &title)
 {
     beginGroup("Favorite");
     bool favo=value(title,0).toBool();
     endGroup();
     return favo;
 }

 QStringList  Setting::favoretedAlbum()
 {
     QStringList list;
     beginGroup("Favorite");
     list= allKeys();
     endGroup();
     return list;
 }
