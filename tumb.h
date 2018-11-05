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

#ifndef TUMB_H
#define TUMB_H

#include "defines.h"
#include <QObject>
#include <QIcon>
#include <QMap>

#define I_ARTIST 1
#define I_Album  2
#define I_Genre  3
#define I_PLAY   4
#define I_PAUSE  5
#define I_ADD    6
#define I_ADD_ALBUM    7
#define I_PANE_HIDE    8
#define I_PANE_SHOW    9
#define I_NEXT   10
#define I_PREV   11
#define I_IMG    12
#define I_START    13
#define I_STARTED   14
#define I_VOLUME   15
#define I_VOLUME_OF    16
#define I_LIST_REMOVE    17
#define I_CLEAN    18
#define I_M_SHUFLE    19
#define I_M_SEQUEN    20
#define I_M_REPEAT    21
#define I_M_REPEAT_ONE    22
#define I_M_PLAY_ONE    23
#define I_AROW    24
#define I_AROW_RTL    25
#define I_FAVO    26
#define I_MENU    27
#define I_PROPERTIES    28
#define I_ALBUM_SMALL    29
#define I_OPTIONS    30
#define I_CLEAR    31
#define I_QUIT    32
#define I_Close    33
#define I_FIND    34
class Tumb
{
public:
    Tumb();
    //!
     static Tumb *instance();
     static QIcon iconAlbum(const QString &title, const QString &dirPath);
     static QString imageAlbumPath(const QString &title, const QString &dirPath);
     static QIcon iconnArtist(const QString &title);
     static QString imageArtiPath(const QString &title);
     static QIcon iconColorized(QString file, QColor color);
     static QIcon icon(int name);

public slots:
   static void setSize(int size){instance()->m_size=size;}
//   static void setIconColor(QColor col,QColor coled)
//   {instance()->m_color=col;instance()->m_colorEd=coled;}
private:
     QMap<QString,QString>*mapIcons;
int m_size;
//QColor m_color;
//QColor m_colorEd;
};

#endif // TUMB_H
