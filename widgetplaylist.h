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

#ifndef WIDGETPLAYLIST_H
#define WIDGETPLAYLIST_H
//#include "playlistview.h"
#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QListView>
#include <QMenu>

class WidgetPlayList : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetPlayList(QWidget *parent = nullptr);

    QListView *playListView(){return mPlayListView;}
signals:
    void rmovePlaylistItem(QModelIndex);
    void movCurentItem(int,int);
    void cleanList();
    void playbackModeChanged(int);
    void getPropperty(bool,const QString &);
public slots:
  //  void setupIcons();
private slots:
   void fileProperies();
    void   editCurent();
    void moveItemUp();
    void moveItemDown();
   // void setPlayMode(QAction *action);
    void setCustomMenu(const QPoint &pos);
private:

    QToolButton *tbRemoveItem;

    QToolButton *tbMoveItemUp;
    QToolButton *tbMoveItemDown;
    QToolButton *tbCleanList;
    QToolButton *tbPlayMode;

    QListView *mPlayListView;


//    QActionGroup *actGroup ;
//    QAction *actPlayOne;
//    QAction *actRepeatOne;
//    QAction *actSquent;
//    QAction *actRepeatAlbum;
//    QAction *actRandom;

};

#endif // WIDGETPLAYLIST_H
