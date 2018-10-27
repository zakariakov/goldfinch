#ifndef DEFINES_H
#define DEFINES_H

#include <QDir>
#include <QStandardPaths>
#define D_CACHE          QDir::homePath()+"/.cache/qplayer"
#define D_ARTIST_CACHE   D_CACHE+"/artistArt"
#define D_ALBUM_CACHE    D_CACHE+"/albmArt"
#define D_Picture        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
#define D_DMUSIC        QStandardPaths::writableLocation(QStandardPaths::MusicLocation)

#define USER_ID               Qt::UserRole
#define USER_RATED            Qt::UserRole+1
#define USER_CHILD_ID         Qt::UserRole+2
#define USER_PARENT_NAME      Qt::UserRole+3
#define USER_TITLE            Qt::UserRole+5 //! بيانات العنوان
#define USER_IMGPATH          Qt::UserRole+4
#define USER_FAVORITE         Qt::UserRole+6

#define  D_MARGINS   3

//! معرف الحالي


#define  MAP_ID    "Id"
#define  MAP_PID    "PId"
#define  MAP_GID   "GId"
#define  MAP_CHILD "Child"
#define  MAP_TITLE "Title"
#define  MAP_PTITLE "PTitle"
#define  MAP_GTITLE "GTitle"
#define  MAP_IMG "Img"
#define  MAP_IS_FAVO "IsFavo"

#define CAT_ID     0
#define CAT_TITLE  1        //! معرف العنوان
#define CAT_ARTIST 2
#define CAT_ALBUM  3
#define CAT_GENRE  4
#define CAT_PATH   5
#define CAT_DURATION   6
#define CAT_RATED  7
//#define CAT_TIME   8
//#define CAT_ALBUMRATED  9
#define CAT_NULL   99

#define COLM_ID     "id"        //0
#define COLM_TITLE  "title"     //1
#define COLM_ARTIST "artist"    //2
#define COLM_ALBUM  "album"     //3
#define COLM_GENRE  "genre"     //4
#define COLM_PATH   "path"      //5
#define COLM_DURATION   "duration"      //6
#define COLM_RATED  "favo"      //7
//#define COLM_TIME   "tags"      //8
//id=0 1=title 2=artist  3=album 4=genre  5=path 6=icon 7=favo 7=tags

#endif // DEFINES_H
