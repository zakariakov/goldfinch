#include "widgetplaylist.h"
#include "tumb.h"
#include <QLayout>
#include <QDebug>
#include <QAction>
WidgetPlayList::WidgetPlayList(QWidget *parent) : QWidget(parent)
{

    //![0]
    mPlayListView=new QListView;
    tbRemoveItem=new QToolButton;
    tbMoveItemUp=new QToolButton;
    tbMoveItemDown=new QToolButton;
    tbCleanList=new QToolButton;
    tbPlayMode=new QToolButton;
    //![1]


    mPlayListView->setFrameShape(QFrame::NoFrame);
    mPlayListView->setIconSize(QSize(12,12));


    tbRemoveItem->setIconSize(QSize(16,16));
    tbRemoveItem->setAutoRaise(true);
    //  tbRemoveItem->setIcon(removeIcon);
    connect(tbRemoveItem,SIGNAL(clicked()),SLOT(rmoveItem()));


    tbMoveItemUp->setIconSize(QSize(16,16));
    tbMoveItemUp->setAutoRaise(true);
    //tbMoveItemUp->setIcon(Tumb::icon(I_LIST_REMOVE,col));
    tbMoveItemUp->setArrowType(Qt::UpArrow);
    connect(tbMoveItemUp,SIGNAL(clicked()),SLOT(moveItemUp()));


    tbMoveItemDown->setIconSize(QSize(16,16));
    tbMoveItemDown->setAutoRaise(true);
    // tbMoveItemDown->setIcon(Tumb::icon(I_LIST_REMOVE,col));
    tbMoveItemDown->setArrowType(Qt::DownArrow);
    connect(tbMoveItemDown,SIGNAL(clicked()),SLOT(moveItemDown()));


    tbCleanList->setIconSize(QSize(16,16));
    tbCleanList->setAutoRaise(true);
    // tbCleanList->setIcon(cleanIcon);
    connect(tbCleanList,SIGNAL(clicked()),SIGNAL(cleanList()));


    QMenu *mnuPlayMode=new QMenu;

    actPlayOne=mnuPlayMode->addAction(tr("CurrentItemOnce"));
    // actPlayOne->setIcon(playoneIcon);
    actPlayOne->setData(0);
    connect(actPlayOne,SIGNAL(triggered()),this,SLOT(setPlayMode()));

    actRepeatOne=mnuPlayMode->addAction(tr("CurrentItemInLoop"));
    //  actRepeatOne->setIcon(repeatoneIcon);
    actRepeatOne->setData(1);
    connect(actRepeatOne,SIGNAL(triggered()),this,SLOT(setPlayMode()));

    actSquent=mnuPlayMode->addAction(tr("Sequential"));
    // actSquent->setIcon(squentialIcon);
    actSquent->setData(2);
    connect(actSquent,SIGNAL(triggered()),this,SLOT(setPlayMode()));

    actRepeatAlbum=mnuPlayMode->addAction(tr("Loop"));
    // actRepeatAlbum->setIcon(repaeatAlbumIcon);
    actRepeatAlbum->setData(3);
    connect(actRepeatAlbum,SIGNAL(triggered()),this,SLOT(setPlayMode()));

    actRandom=mnuPlayMode->addAction(tr("Random"));
    //  actRandom->setIcon(shufleAlbumIcon);
    actRandom->setData(4);
    connect(actRandom,SIGNAL(triggered()),this,SLOT(setPlayMode()));

    tbPlayMode->setIcon(Tumb::icon(I_M_SEQUEN));
    tbPlayMode->setPopupMode(QToolButton::InstantPopup);
    tbPlayMode->setIconSize(QSize(16,16));
    tbPlayMode->setAutoRaise(true);

    tbPlayMode->setMenu(mnuPlayMode);

    setupIcons();
    //------------- LAYOUTS -----------------

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setMargin(0);

    hLayout->addWidget(tbPlayMode);
    hLayout->addStretch();
    hLayout->addWidget(tbMoveItemDown);
    hLayout->addWidget(tbMoveItemUp);
    hLayout->addSpacing(10);
    hLayout->addWidget(tbCleanList);
    hLayout->addWidget(tbRemoveItem);
    hLayout->addSpacing(5);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mPlayListView);

    setLayout(vLayout);

}
void WidgetPlayList::setupIcons()
{
    tbRemoveItem->setIcon(Tumb::icon(I_LIST_REMOVE));
    tbCleanList->setIcon(Tumb::icon(I_CLEAN));
    //playModeIcon=Tumb::icon(I_M_SEQUEN);

    actPlayOne->setIcon(Tumb::icon(I_M_PLAY_ONE));
    actRepeatOne->setIcon(Tumb::icon(I_M_REPEAT_ONE));
    actSquent->setIcon(Tumb::icon(I_M_SEQUEN));
    actRepeatAlbum->setIcon(Tumb::icon(I_M_REPEAT));
    actRandom->setIcon(Tumb::icon(I_M_SHUFLE));
}

void WidgetPlayList::rmoveItem()
{
    //TODO Add selectedIndexes
    //QModelIndexList list= mPlayListView->selectedIndexes();

    QModelIndex idx=mPlayListView->currentIndex();
    if(!idx.isValid())return;

    emit rmovePlaylistItem(idx);

}

void WidgetPlayList::moveItemUp()
{
    QModelIndex idx=mPlayListView->currentIndex();
    if(!idx.isValid())return;

     int row=idx.row();
    QModelIndex idxUp=mPlayListView->model()->index(row-1,0);
     if(!idxUp.isValid())return;

    emit movCurentItem(row,row-1);
}

void WidgetPlayList::moveItemDown()
{
    QModelIndex idx=mPlayListView->currentIndex();
    if(!idx.isValid())return;

    int row=idx.row();
    QModelIndex idxDown=mPlayListView->model()->index(row+1,0);
    if(!idxDown.isValid())return;

    emit movCurentItem(row,row+1);
}

void WidgetPlayList::setPlayMode()
{

    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
    {

        int data=action->data().toInt();
        qDebug()<<"playmode="<<data;
        emit playbackModeChanged(data);
        tbPlayMode->setIcon(action->icon());

    }

}
