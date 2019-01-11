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

#include "treeviewaudio.h"

#include "database.h"
#include <QHeaderView>
#include <QResizeEvent>
//#include <QDebug>
#include <QMenu>
#include <QProcess>
#include <QSettings>
TreeViewAudio::TreeViewAudio()
{
    setRootIsDecorated(false);
    setFrameShape(QFrame::NoFrame);
    setColumnWidth(1,16);
    header()->setMinimumSectionSize(24);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    header()->setStretchLastSection(false);
    header()->setCascadingSectionResizes(true);
    setHeaderHidden(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenu(QPoint)));
    setContextMenuPolicy(Qt::CustomContextMenu);
    if(isRightToLeft())
    setTextElideMode(Qt::ElideNone);
}

void TreeViewAudio::resizeEvent(QResizeEvent *event)
{
    header()->setSectionResizeMode(2,QHeaderView::Stretch);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    header()->resizeSection(2,event->size().width());
}

void TreeViewAudio::customContextMenu(QPoint )
{

    QModelIndex idx=   selectionModel()->currentIndex();
    if(!idx.isValid()) return;

    QMenu mnu;
    mnu.addAction(tr("Add to playlist"),this,SLOT(addCurentToPLaylist()));
    mnu.addAction(tr("Favorite/Unfavorite"),this,SLOT(favoriteCurent()));
    mnu.addSeparator();
    mnu.addAction(tr("Edit tags"),this,SLOT(editCurent()));
    mnu.addSeparator();
    mnu.addAction(tr("Remove"),this,SLOT(removeFile()));
    mnu.addSeparator();
    mnu.addAction(tr("Properties"),this,SLOT(fileProperies()));
    mnu.exec(QCursor::pos());
}

void TreeViewAudio::addCurentToPLaylist()
{
    int row=   selectionModel()->currentIndex().row();
     QModelIndex idx= model()->index(row,HIDER_ADD);
    if(!idx.isValid()) return;

    emit treeAudioClicked(idx);
}

void TreeViewAudio::favoriteCurent()
{
    int row=   selectionModel()->currentIndex().row();
     QModelIndex idx= model()->index(row,HIDER_FAVO);
    if(!idx.isValid()) return;

    emit treeAudioClicked(idx);

}
void TreeViewAudio::editCurent()
{
//    int row=   selectionModel()->currentIndex().row();
//     QModelIndex idx= model()->index(row,HIDER_TITLE);

//     if(!idx.isValid()) return;

//    QString path=idx.data(Qt::UserRole).toString();
//    QProcess p;
//    p.startDetached("easytag",QStringList()<<path);
    int row=   selectionModel()->currentIndex().row();
  QString file=model()->index(row,HIDER_TITLE).data(Qt::UserRole).toString();
    emit getProperty(false,file);
}

void TreeViewAudio::fileProperies()
{
     int row=   selectionModel()->currentIndex().row();
   QString file=model()->index(row,HIDER_TITLE).data(Qt::UserRole).toString();

emit getProperty(true,file);

}

void TreeViewAudio::removeFile()
{
    int row=   selectionModel()->currentIndex().row();
    QModelIndex idx= model()->index(row,HIDER_TITLE);

    if(!idx.isValid()) return;

    QString path=idx.data(Qt::UserRole).toString();
    if(DataBase::removeSong(path)){
         QSettings s(CACHE+"/filesinfo",QSettings::IniFormat);
        s.remove(path);
        emit updateCurent();
    }
}
