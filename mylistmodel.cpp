#include "mylistmodel.h"
#include "listeditor.h"
#include "tumb.h"
#include <QSettings>
//#include <QDebug>
MyListModel::MyListModel( QObject *parent) :
    QStandardItemModel(parent)
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

    QStringList list=DataBase::chargeChild(child,curent,curentName,pId,pName);

    foreach(QString title,list)
    {

        if(title.isEmpty()||title.isNull())
            continue;

        QStandardItem* item = new QStandardItem();
        item->setText(title);
        item->setData(title,USER_TITLE);
        item->setData(child,USER_ID);
        item->setData(COL_I_NULL,USER_CHILD_ID);
        item->setData(curentName,USER_PARENT_NAME);
        //! ----------------------* IDITOR *---------------------------//
        item->setData(QVariant::fromValue(Iditoring()),Qt::EditRole);

        int favo=Setting::albumIsFavorited(title);
        QString imgPath=Setting::albumImgPath(title);

        //  QString imgPath=s.value(title).toString();
        item->setData(favo,USER_RATED);
        item->setData(imgPath,USER_IMGPATH);
        invisibleRootItem()-> appendRow(item);

    }

    sort(0);
    invisibleRootItem()->sortChildren(0);
}

void MyListModel::chargeFavoritedAlbum()
{
    clear();

    QStringList list=Setting::favoretedAlbum();

    foreach(QString title,list)
    {


        if(title.isEmpty()||title.isNull())
            continue;

        QStandardItem* item = new QStandardItem();
        item->setText(title);
        item->setData(title,USER_TITLE);
        item->setData(COL_I_ALBUM,USER_ID);
        item->setData(COL_I_NULL,USER_CHILD_ID);
        item->setData(true,USER_FAVO_DISPLY);
        // item->setData(title,USER_PARENT_NAME);
        //! ----------------------* IDITOR *---------------------------//
       item->setData(QVariant::fromValue(Iditoring()),Qt::EditRole);

        bool favo=Setting::albumIsFavorited(title);
        QString imgPath=Setting::albumImgPath(title);
        //  QString imgPath=s.value(title).toString();
        item->setData(favo,USER_RATED);
        item->setData(imgPath,USER_IMGPATH);
        invisibleRootItem()-> appendRow(item);


    }

    sort(0);
    invisibleRootItem()->sortChildren(0);

}

//______________________________________________________________________________________________________CHILDRENS
void MyListModel::chargeAudios(QString name, int colm,
                               QString pName, int pColm,
                               QString pPname, int pPcolm)
{

    clear();

    QList<QVariantMap> list=DataBase::chargeAudios
            (name,colm,pName,pColm,pPname,pPcolm);


    int count=list.count();
    for (int i = 0; i < count; ++i) {

        QVariantMap map=list.at(i);

        // 0  HIDER_ADD
        QStandardItem* itemAdd = new QStandardItem();
        itemAdd->setEditable(false);
        itemAdd->setData(tr("Add curent to playlist"),USER_TITLE);
        itemAdd->setIcon(Tumb::icon(I_ADD)/*QIcon(":/icons/play")*/);

        // 1 HIDER_FAVO
        QStandardItem* itemFavo = new QStandardItem();
        int favo=map.value(COL_S_RATED).toInt();
        itemFavo->setIcon(!favo ? Tumb::icon(I_START):Tumb::icon(I_STARTED));
        itemFavo->setData(tr("Add curent to fvorite"),USER_TITLE);
        itemFavo->setData(favo,USER_RATED);
        itemFavo->setEditable(false);

        // 2 HIDER_TITLE
        QStandardItem* itemTitle = new QStandardItem(map.value(COL_S_TITLE).toString());
        itemTitle->setData(map.value(COL_S_PATH),Qt::UserRole);
        //item->setIcon(Tumb::icon(I_Album));
        itemTitle->setEditable(false);

        // 3  HIDER_ARTIST
        QStandardItem* itemArt = new QStandardItem(map.value(COL_S_ARTIST).toString());
        itemArt->setEditable(false);

        // 4  HIDER_ALBUM
        QStandardItem* itemAlb = new QStandardItem(map.value(COL_S_ALBUM).toString());
        itemAlb->setEditable(false);

        // 5  HIDER_TIME
        QStandardItem* itemDur = new QStandardItem(map.value(COL_S_DURATION).toString());
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
     invisibleRootItem()->sortChildren(HIDER_TITLE);
    sort(HIDER_TITLE);


}

void MyListModel::searchAudios(int col ,const QString &text)
{

    clear();

    QList<QVariantMap> list=DataBase::searchAudios(col,text);

    int count=list.count();
    for (int i = 0; i < count; ++i) {

        QVariantMap map=list.at(i);

        // 0  HIDER_ADD
        QStandardItem* itemAdd = new QStandardItem();
        itemAdd->setEditable(false);
        itemAdd->setData(tr("Add curent to playlist"),USER_TITLE);
        itemAdd->setIcon(Tumb::icon(I_ADD)/*QIcon(":/icons/play")*/);

        // 1 HIDER_FAVO
        QStandardItem* itemFavo = new QStandardItem();
        int favo=map.value(COL_S_RATED).toInt();
        itemFavo->setIcon(!favo ? Tumb::icon(I_START):Tumb::icon(I_STARTED));
        itemFavo->setData(tr("Add curent to fvorite"),USER_TITLE);
        itemFavo->setData(favo,USER_RATED);
        itemFavo->setEditable(false);

        // 2 HIDER_TITLE
        QStandardItem* itemTitle = new QStandardItem(map.value(COL_S_TITLE).toString());
        itemTitle->setData(map.value(COL_S_PATH),Qt::UserRole);
        //item->setIcon(Tumb::icon(I_Album));
        itemTitle->setEditable(false);

        // 3  HIDER_ARTIST
        QStandardItem* itemArt = new QStandardItem(map.value(COL_S_ARTIST).toString());
        itemArt->setEditable(false);

        // 4  HIDER_ALBUM
        QStandardItem* itemAlb = new QStandardItem(map.value(COL_S_ALBUM).toString());
        itemAlb->setEditable(false);

        // 5  HIDER_TIME
        QStandardItem* itemDur = new QStandardItem(map.value(COL_S_DURATION).toString());
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
     invisibleRootItem()->sortChildren(HIDER_TITLE);
    sort(HIDER_TITLE);

}

