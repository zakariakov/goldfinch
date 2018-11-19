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

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QObject>
#include <QListView>

class ListView : public QListView
{
    Q_OBJECT
public:
    ListView();
    QModelIndex curHighlilghtedIndex(){
        if(mOldIndex.isValid())
        return mOldIndex;
        return QModelIndex();
    }
private slots:
 //   Q_SIGNALS:
  void  onItemEntered(const QModelIndex &index);
  void selectionHasChanged();

private:
  QModelIndex mOldIndex;
  //!
  QItemSelectionModel *listSelectionModel;
};

#endif // LISTVIEW_H
