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

#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H
#include "setting.h"
#include "database.h"
#include <QStandardItemModel>
#include <QObject>
#include <QUrl>
class MyListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    MyListModel(QObject *parent = nullptr);

    void chargeCategory(QString curentName, int curent, int child, int pId, QString pName);
    void chargeAudios(QString name, int colm,
                      QString pName=QString(), int pColm=0,
                      QString pPname=QString(), int pPcolm=0);
void chargeFavoritedAlbum();
void searchAudios(int col ,const QString &text);

  QIcon icon(const QModelIndex &index){return item(index.row())->icon();}
protected:

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &) const{return 6;}

private:
    Setting *mSetting;
  //  DataBase *mDataBase;
 //   QList<QStandardItem*>listItem;
};

#endif // MYLISTMODEL_H
