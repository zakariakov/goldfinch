#include "mylistmodel.h"
#include "listeditor.h"
#include "tumb.h"
#include <QSettings>
#include <QDebug>
MyListModel::MyListModel(Setting *setting, DataBase *data, QObject *parent) :
    QStandardItemModel(parent),mSetting(setting),mDataBase(data)
{

}

QVariant MyListModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())  return QVariant();

    if(role == Qt::ToolTipRole){

        if(index.column()==0 ||index.column()==1 ){
            return index.data(USER_TITLE);
        }

        if(index.column()==2||index.column()==3||index.column()==4){
            return index.data();
        }

    }
    return QStandardItemModel::data(index,role);
}
//____________________________________________________________________________________
QVariant MyListModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(orientation==Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section)
            {
            case 0: return QString();
            case 1: return QString();
            case 2: return tr("Title");
            case 3: return tr("Artist");
            case 4: return tr("Album");
            case 5: return tr("Duration");
            default: return QVariant();
            }

    return QVariant();

}

void MyListModel::chargeCategory(QString curentName, int curent, int child,int pId,QString pName)
{

    clear();

    QStringList list=mDataBase->chargeChild(child,curent,curentName,pId,pName);

    foreach(QString title,list)
    {


        if(title.isEmpty()||title.isNull())
            continue;

        QStandardItem* item = new QStandardItem();
        item->setText(title);
        item->setData(title,USER_TITLE);
        item->setData(child,USER_ID);
        item->setData(CAT_NULL,USER_CHILD_ID);
        item->setData(curentName,USER_PARENT_NAME);
        //! ----------------------* IDITOR *---------------------------//
        item->setData(QVariant::fromValue(Iditoring()),Qt::EditRole);

        int favo=mSetting->albumIsFavorited(title);
        QString imgPath=mSetting->albumImgPath(title);
        //  QString imgPath=s.value(title).toString();
        item->setData(favo,USER_RATED);
        item->setData(imgPath,USER_IMGPATH);
        invisibleRootItem()-> appendRow(item);


    }

    sort(0);

}

void MyListModel::chargeFavoritedAlbum()
{
      clear();

     QStringList list=mSetting->favoretedAlbum();

     foreach(QString title,list)
     {


         if(title.isEmpty()||title.isNull())
             continue;

         QStandardItem* item = new QStandardItem();
         item->setText(title);
         item->setData(title,USER_TITLE);
        item->setData(CAT_ALBUM,USER_ID);
         item->setData(CAT_NULL,USER_CHILD_ID);
           item->setData(true,USER_FAVORITE);
        // item->setData(title,USER_PARENT_NAME);
          //! ----------------------* IDITOR *---------------------------//
         item->setData(QVariant::fromValue(Iditoring()),Qt::EditRole);

         bool favo=mSetting->albumIsFavorited(title);
         QString imgPath=mSetting->albumImgPath(title);
         //  QString imgPath=s.value(title).toString();
         item->setData(favo,USER_RATED);
         item->setData(imgPath,USER_IMGPATH);
         invisibleRootItem()-> appendRow(item);


     }

     sort(0);


}


//______________________________________________________________________________________________________CHILDRENS
void MyListModel::chargeAudios(QString name, int colm,
                               QString pName, int pColm,
                               QString pPname, int pPcolm)
{

    clear();

    QList<QVariantMap> list=mDataBase->chargeAudios
            (name,colm,pName,pColm,pPname,pPcolm);


    int count=list.count();
    for (int i = 0; i < count; ++i) {

        QVariantMap map=list.at(i);

        // 0
        QStandardItem* itemAdd = new QStandardItem();
        itemAdd->setEditable(false);
        itemAdd->setData(tr("Add curent to playlist"),USER_TITLE);
        itemAdd->setIcon(Tumb::icon(I_ADD)/*QIcon(":/icons/play")*/);

        // 1
        QStandardItem* itemFavo = new QStandardItem();
        int favo=map.value(COLM_RATED).toInt();
        itemFavo->setIcon(!favo ? Tumb::icon(I_START):Tumb::icon(I_STARTED));
        itemFavo->setData(tr("Add curent to fvorite"),USER_TITLE);
        itemFavo->setData(favo,USER_RATED);
        itemFavo->setEditable(false);

        // 2
        QStandardItem* itemTitle = new QStandardItem(map.value(COLM_TITLE).toString());
        itemTitle->setData(map.value(COLM_PATH),Qt::UserRole);
        //item->setIcon(Tumb::icon(I_Album));
        itemTitle->setEditable(false);

        // 3
        QStandardItem* itemArt = new QStandardItem(map.value(COLM_ARTIST).toString());
        itemArt->setEditable(false);

        // 4
        QStandardItem* itemAlb = new QStandardItem(map.value(COLM_ALBUM).toString());
        itemAlb->setEditable(false);

        QStandardItem* itemDur = new QStandardItem(map.value(COLM_DURATION).toString());
        itemDur->setEditable(false);

        invisibleRootItem()->appendRow(QList<QStandardItem*>()
                                       <<itemAdd
                                       <<itemFavo
                                       <<itemTitle
                                       <<itemArt
                                       <<itemAlb
                                       <<itemDur
                                       );
    }

    sort(0);

}



