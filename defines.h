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

#ifndef DEFINES_H
#define DEFINES_H

#include <QDir>
#include <QStandardPaths>
#define D_CACHE          QDir::homePath()+"/.cache/qplayer"
#define D_ARTIST_CACHE   D_CACHE+"/artistArt"
#define D_ALBUM_CACHE    D_CACHE+"/albmArt"
#define D_Picture        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
#define D_DMUSIC        QStandardPaths::writableLocation(QStandardPaths::MusicLocation)

#define  D_MARGINS   3

//!  معرفات تخزين البيانات
#define USER_ID               Qt::UserRole   //! بيانات المعرف الحالي
#define USER_RATED            Qt::UserRole+1 //! بيانات المفضلة
#define USER_CHILD_ID         Qt::UserRole+2 //! بيانات معرف الابن
#define USER_PARENT_NAME      Qt::UserRole+3 //! بيانات العنوان الاب
#define USER_TITLE            Qt::UserRole+5 //! بيانات العنوان
#define USER_IMGPATH          Qt::UserRole+4 //! بيانات الصورة
#define USER_FAVO_DISPLY      Qt::UserRole+6 //! لمعرفة هل هو عرض المفضلة

//!  معرفات اعمدة الشجرة
#define HIDER_ADD    0
#define HIDER_FAVO   1
#define HIDER_TITLE  2
#define HIDER_ARTIST 3
#define HIDER_ALBUM  4
#define HIDER_TIME   5

//! معرف بيانات الحالي
#define  MAP_ID      "Id"
#define  MAP_PID     "PId"
#define  MAP_GID     "GId"
#define  MAP_CHILD   "Child"
#define  MAP_TITLE   "Title"
#define  MAP_PTITLE  "PTitle"
#define  MAP_GTITLE  "GTitle"
#define  MAP_IMG     "Img"
#define  MAP_IS_FAVO "IsFavo"

//! معرف اعمدة قاعدة البيانات
#define COL_I_ID         0
#define COL_I_TITLE      1
#define COL_I_ARTIST     2
#define COL_I_ALBUM      3
#define COL_I_GENRE      4
#define COL_I_PATH       5
#define COL_I_DURATION   6
#define COL_I_RATED      7
#define COL_I_MODIF      8
#define COL_I_NULL      99

//! معرف اسماء اعمدة قاعدة البيانات
#define COL_S_ID         "id"        //0
#define COL_S_TITLE      "title"     //1
#define COL_S_ARTIST     "artist"    //2
#define COL_S_ALBUM      "album"     //3
#define COL_S_GENRE      "genre"     //4
#define COL_S_PATH       "path"      //5
#define COL_S_DURATION   "duration"  //6
#define COL_S_RATED      "favo"      //7
#define COL_S_MODIF      "modified"  //8

// id=0 1=title 2=artist  3=album 4=genre  5=path 6=icon 7=favo 7=tags

#endif // DEFINES_H
