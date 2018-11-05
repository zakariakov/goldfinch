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

#ifndef TREEVIEWAUDIO_H
#define TREEVIEWAUDIO_H

#include <QObject>
#include <QTreeView>
class TreeViewAudio : public QTreeView
{
      Q_OBJECT
public:
    TreeViewAudio();

    void resizeEvent(QResizeEvent *event);
public slots:
    void customContextMenu(QPoint);
signals:
   void treeAudioClicked(const QModelIndex);
   void updateCurent();
//   void editCurIndex(const QString);
private slots:
    void addCurentToPLaylist();
    void favoriteCurent();
     void editCurent();
     void fileProperies();
    void removeFile();
signals:
    void treewidthChanged(int );
};

#endif // TREEVIEWAUDIO_H
