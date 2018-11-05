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

#ifndef MYCONTENTMODEL_H
#define MYCONTENTMODEL_H
//#include "defines.h"
#include "database.h"
#include <QStandardItemModel>
#include <QApplication>
#include <QDir>



class MyContentModel : public QStandardItemModel
{
      Q_OBJECT
public:
    explicit MyContentModel( QObject *parent = nullptr);

 QIcon icon(const QModelIndex &index){return item(index.row())->icon();}
protected:

 //   QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  //  int columnCount(const QModelIndex &) const{return 5;}

private:


 // DataBase *mDataBase;
  //  int  mediaMeyaData;
//    int  curGroup;
//     bool isGroup;

public slots:

   void chargeCategory(int root, int child=COL_I_NULL, int children=COL_I_NULL);
 QList<QStandardItem *> addChilds(QString prentName, int parentColumn, int child, int children=COL_I_NULL);

private slots:
//void chargeFavoritedAudio();
void chargeFavoritedAlbum();


};

#endif // MYCONTENTMODEL_H
