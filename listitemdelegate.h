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

#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H
#include "defines.h"
#include <QPainter>
#include <QObject>
#include <QStyledItemDelegate>

class ListItemDelegate : public QStyledItemDelegate
{
     Q_OBJECT
public:
    ListItemDelegate();

    //!
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const ;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const ;


signals:
  void  addAlbum(int);
  void  favoritAlbum(int,bool);
  void  playAlbum(int);
  void  changeImageAlbum(int);

private :
    //!  رسم الايقونة عند الوضع التفصيلي والقياسي
//    void paintDetailView(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!  رسم الايقونة في حالة وضع الايقونات
    void paintIconView(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!  ...جلب حالة الايقونة بين مفعلة وعادية و
    static QIcon::Mode iconModeFromState(QStyle::State state);

    //!   جلب الايقونة
 //   QIcon decoration(const QModelIndex &index) const;


    QMap<QString,QString> *mIconMap;
};

#endif // LISTITEMDELEGATE_H
