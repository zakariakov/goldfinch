#include "treeviewaudio.h"
#include "propertiesfile.h"
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
    mnu.addAction(tr("Properies"),this,SLOT(fileProperies()));
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
    int row=   selectionModel()->currentIndex().row();
     QModelIndex idx= model()->index(row,HIDER_TITLE);

     if(!idx.isValid()) return;

    QString path=idx.data(Qt::UserRole).toString();
    QProcess p;
    p.startDetached("easytag",QStringList()<<path);
}

void TreeViewAudio::fileProperies()
{
    QVariantMap map;
    int row=   selectionModel()->currentIndex().row();

    map["Title"]= model()->index(row,HIDER_TITLE).data().toString();
    map["Album"]= model()->index(row,HIDER_ALBUM).data().toString();
    map["Artist"]=model()->index(row,HIDER_ARTIST).data().toString();
    map["Path"]=  model()->index(row,HIDER_TITLE).data(Qt::UserRole).toString();

    PropertiesFile *dlg=new PropertiesFile;
    dlg->setInformations(map);
    dlg->exec();
    delete dlg;

}

void TreeViewAudio::removeFile()
{
    int row=   selectionModel()->currentIndex().row();
    QModelIndex idx= model()->index(row,HIDER_TITLE);

    if(!idx.isValid()) return;

    QString path=idx.data(Qt::UserRole).toString();
    if(DataBase::removeSong(path)){
         QSettings s(D_CACHE+"/filesinfo",QSettings::IniFormat);
        s.remove(path);
        emit updateCurent();
    }
}
