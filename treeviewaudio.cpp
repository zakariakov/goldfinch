#include "treeviewaudio.h"
#include <QHeaderView>
#include <QResizeEvent>
#include <QDebug>
#include <QMenu>

TreeViewAudio::TreeViewAudio()
{
setRootIsDecorated(false);
setFrameShape(QFrame::NoFrame);
 setColumnWidth(1,16);
//header()->setDefaultSectionSize(100);
//header()->setMaximumSectionSize(100);
header()->setMinimumSectionSize(24);
     setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

header()->setStretchLastSection(false);
//setColumnWidth(0,200);
//setColumnWidth(0,1);
//setColumnWidth(2,50);
//setColumnWidth(3,50);
//setColumnWidth(4,50);
header()->setCascadingSectionResizes(true);
//header()->setSectionResizeMode(0,QHeaderView::Stretch);
//header()->setSectionResizeMode(1,QHeaderView::Fixed);
//header()->setSectionResizeMode(2,QHeaderView::Fixed);
//header()->setSectionResizeMode(3,QHeaderView::Fixed);
//setMinimumWidth(250);

//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setHeaderHidden(true);
setEditTriggers(QAbstractItemView::AllEditTriggers);

 connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenu(QPoint)));
 setContextMenuPolicy(Qt::CustomContextMenu);
}

void TreeViewAudio::resizeEvent(QResizeEvent *event)
{
    header()->setSectionResizeMode(2,QHeaderView::Stretch);
  setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
   header()->resizeSection(2,event->size().width());

//   qDebug()<<   event->oldSize()<<event->size()<<width();;
//    header()->setSectionResizeMode(2,QHeaderView::Stretch);
//   emit treewidthChanged(size().width());

}

void TreeViewAudio::customContextMenu(QPoint )
{

 QModelIndex idx=   selectionModel()->currentIndex();
 if(!idx.isValid()) return;

 QMenu mnu;
 QAction *act=mnu.addAction(tr("Add to playlist"),this,SLOT(addCurentToPLaylist()));
 act=mnu.addAction(tr("Favorite / Unfavorite"),this,SLOT(favoriteCurent()));
 mnu.addSeparator();
 act=mnu.addAction(tr("Edit tags"),this,SLOT(editCurent()));

 mnu.exec(QCursor::pos());
}

void TreeViewAudio::addCurentToPLaylist()
{
    int row=   selectionModel()->currentIndex().row();
     QModelIndex idx= model()->index(row,0);
    if(!idx.isValid()) return;

    emit treeAudioClicked(idx);
}

void TreeViewAudio::favoriteCurent()
{
    int row=   selectionModel()->currentIndex().row();
     QModelIndex idx= model()->index(row,1);
    if(!idx.isValid()) return;

    emit treeAudioClicked(idx);

}
void TreeViewAudio::editCurent()
{
    int row=   selectionModel()->currentIndex().row();
     QModelIndex idx= model()->index(row,2);

     if(!idx.isValid()) return;

    QString path=idx.data(Qt::UserRole).toString();
    emit editCurIndex(path);
}
