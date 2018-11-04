#include "widgetplaylist.h"
#include "tumb.h"
#include <QLayout>
//#include <QDebug>
#include <QAction>
WidgetPlayList::WidgetPlayList(QWidget *parent) : QWidget(parent)
{

    actGroup =      new QActionGroup(this);
    mPlayListView=  new QListView;
    tbRemoveItem=   new QToolButton;
    tbMoveItemUp=   new QToolButton;
    tbMoveItemDown= new QToolButton;
    tbCleanList=    new QToolButton;
    tbPlayMode=     new QToolButton;

    mPlayListView->setFrameShape(QFrame::NoFrame);
    mPlayListView->setIconSize(QSize(16,16));

    tbRemoveItem->setIconSize(QSize(16,16));
    tbRemoveItem->setAutoRaise(true);
    tbRemoveItem->setToolTip(tr("Remove Current "));
    connect(tbRemoveItem,SIGNAL(clicked()),SLOT(rmoveItem()));

    tbMoveItemUp->setIconSize(QSize(16,16));
    tbMoveItemUp->setAutoRaise(true);
    tbMoveItemUp->setArrowType(Qt::UpArrow);
    tbMoveItemUp->setToolTip(tr("Move Up Current "));
    connect(tbMoveItemUp,SIGNAL(clicked()),SLOT(moveItemUp()));

    tbMoveItemDown->setIconSize(QSize(16,16));
    tbMoveItemDown->setAutoRaise(true);
    tbMoveItemDown->setToolTip(tr("Move Down Current "));
    tbMoveItemDown->setArrowType(Qt::DownArrow);
    connect(tbMoveItemDown,SIGNAL(clicked()),SLOT(moveItemDown()));

    tbCleanList->setIconSize(QSize(16,16));
    tbCleanList->setAutoRaise(true);
    tbCleanList->setToolTip(tr("Clear Plylist"));
    connect(tbCleanList,SIGNAL(clicked()),SIGNAL(cleanList()));

    QMenu *mnuPlayMode=new QMenu;

    actPlayOne=mnuPlayMode->addAction(tr("Current Once"));
    actPlayOne->setData(0);
    actPlayOne->setCheckable(true);

    actRepeatOne=mnuPlayMode->addAction(tr("Current Loop"));
    actRepeatOne->setData(1);
    actRepeatOne->setCheckable(true);

    actSquent=mnuPlayMode->addAction(tr("Sequential"));
    actSquent->setData(2);
    actSquent->setCheckable(true);

    actRepeatAlbum=mnuPlayMode->addAction(tr("Plylist Loop"));
    actRepeatAlbum->setData(3);
    actRepeatAlbum->setCheckable(true);

    actRandom=mnuPlayMode->addAction(tr("Random"));
    actRandom->setData(4);
    actRandom->setCheckable(true);

    actGroup->addAction(actPlayOne);
    actGroup->addAction(actRepeatOne);
    actGroup->addAction(actSquent);
    actGroup->addAction(actRepeatAlbum);
    actGroup->addAction(actRandom);
    connect(actGroup,&QActionGroup::triggered,this,&WidgetPlayList::setPlayMode);

    actSquent->setChecked(true);

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

void WidgetPlayList::setPlayMode(QAction *action)
{

   // QAction *action = qobject_cast<QAction *>(sender());
    if(action)
    {
        int data=action->data().toInt();
     //   qDebug()<<"playmode="<<data;
        emit playbackModeChanged(data);
        tbPlayMode->setIcon(action->icon());

    }

}
