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

#include "treeitemdelegate.h"
#include "listeditor.h"
#include "tumb.h"
#include <QPainter>
TreeItemDelegate::TreeItemDelegate()
{

}

void TreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;


    if(  option.state & QStyle::State_MouseOver )  {

        painter->setOpacity(0.3);
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setOpacity(1.0);
    }

    QStyledItemDelegate::paint(painter,option,index);
}

// ______________________________________________________________________________
QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(!index.isValid()) return  QSize();

    return QStyledItemDelegate::sizeHint(option,index);
}


