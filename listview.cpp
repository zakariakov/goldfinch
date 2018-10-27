#include "listview.h"
#include <QDebug>
ListView::ListView()
{
setFrameShape(QFrame::NoFrame);
    setMouseTracking(true);
    setFlow(QListView::LeftToRight);
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    setWrapping(true);

     setViewMode(QListView::IconMode);
  //   setIconSize(QSize(128,128));
     setSpacing(5);

 // ----------------------* IDITOR *---------------------------
setEditTriggers(QAbstractItemView::NoEditTriggers);
connect(this,SIGNAL(entered(QModelIndex)),this,SLOT(onItemEntered(QModelIndex)));
connect(this,SIGNAL(viewportEntered()),this,SLOT(selectionHasChanged()));

}

 // ----------------------* IDITOR *---------------------------
void  ListView::onItemEntered(const QModelIndex &index)
{

    this->closePersistentEditor(mOldIndex);
    this->openPersistentEditor(index);
    mOldIndex=index;

}
 // ----------------------* IDITOR *---------------------------
void  ListView::selectionHasChanged()
{

//    qDebug()<<"==========="<<row;
//    int col=5;
//QModelIndex indx=this->model()->index(row,5);
    if(mOldIndex.isValid())
    this->closePersistentEditor(mOldIndex);

  //  this->edit(index);

}
