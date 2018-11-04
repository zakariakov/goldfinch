#include "mycontentmodel.h"
#include "tumb.h"
//#include <QDebug>
#include <QSettings>
MyContentModel::MyContentModel (QObject *parent) :
    QStandardItemModel(parent)
{

}


//_________________________________________________________________________ROOT


void MyContentModel::chargeCategory(int root, int child, int children)
{

    clear();
    QStandardItem* itemFavorite = new QStandardItem();
    itemFavorite->setText(tr("Favorite Song"));
    itemFavorite->setData("Favorite Song",USER_TITLE);
    itemFavorite->setData(COL_I_RATED,USER_ID);
    itemFavorite->setData(COL_I_NULL,USER_CHILD_ID);
     itemFavorite->setData(true,USER_FAVO_DISPLY);
    itemFavorite->setIcon(Tumb::icon(I_STARTED));


    QStandardItem* itemAlbum = new QStandardItem();
    itemAlbum->setText(tr("Favorite Albums"));
    itemAlbum->setData("Favorite Albums",USER_TITLE);
    itemAlbum->setData(/*CAT_ALBUMRATED*/COL_I_ALBUM,USER_ID);
    itemAlbum->setData(COL_I_ALBUM,USER_CHILD_ID);
    itemAlbum->setIcon(Tumb::icon(I_STARTED));
    itemAlbum->setData(true,USER_FAVO_DISPLY);
    QStringList list=DataBase::chargeRoot(root);

    foreach(QString name,list)
    {

        if(name.isEmpty()||name.isNull())
            continue;

        QStandardItem* item = new QStandardItem();
        item->setText(name);
        item->setData(name,USER_TITLE);
        item->setData(root,USER_ID);
        item->setData(child,USER_CHILD_ID);
        if(root==COL_I_ALBUM){
            QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
            QString imgPath=s.value(name).toString();
            item->setData(imgPath,USER_IMGPATH);
            //  item->setIcon(QIcon(":/icons/cover-16"));
            item->setIcon(Tumb::icon(I_ALBUM_SMALL));
        }else  if(root==COL_I_ARTIST){
            item->setIcon(Tumb::icon(I_ARTIST));
        }else
            item->setIcon(Tumb::icon(I_Genre));


        invisibleRootItem()-> appendRow(item);


        if(child!=COL_I_NULL){
            item->sortChildren(0);
            item->appendRows(addChilds(name,root,child,children));

        }

    }
 sort(0);
 invisibleRootItem()->insertRow(0,itemAlbum);
 invisibleRootItem()->insertRow(0,itemFavorite);
}

void MyContentModel::chargeFavoritedAlbum()
{
     QStringList list=DataBase::chargeFavoritedAlbum();
     foreach(QString name,list)
     {


         if(name.isEmpty()||name.isNull())
             continue;
         QStandardItem* item = new QStandardItem();
         item->setText(name);
         item->setData(name,USER_TITLE);
         item->setData(COL_I_ALBUM,USER_ID);
         item->setData(COL_I_NULL,USER_CHILD_ID);

         item->setIcon(Tumb::icon(I_Genre));


         invisibleRootItem()-> appendRow(item);




     }
  sort(0);

}
//______________________________________________________________________________________________________CHILDS
QList<QStandardItem *> MyContentModel::addChilds(QString prentName, int parentColumn, int child, int children)
{


    QStringList list=DataBase::chargeChild(child,parentColumn,prentName);
    QList<QStandardItem *> items;
    foreach(QString name,list)
    {


        QStandardItem* item = new QStandardItem();

        //   item->insertColumns(0,5);
        item->setText(name);
        item->setData(name,USER_TITLE);
        item->setData(child,USER_ID);
        item->setData(children,USER_CHILD_ID);
        //         item->setData(parentColumn,USER_PARENT_ID);
        item->setData(prentName,USER_PARENT_NAME);
        if(child==COL_I_ALBUM){
            QSettings s(D_CACHE+"/albums",QSettings::IniFormat);
            QString imgPath=s.value(name).toString();
            item->setData(imgPath,USER_IMGPATH);
            item->setIcon(Tumb::icon(I_ALBUM_SMALL));
        }else  if(child==COL_I_ARTIST){
            item->setIcon(Tumb::icon(I_ARTIST));
        }else
            item->setIcon(Tumb::icon(I_Genre));

        items.append(item);

        item->appendRows(addChilds(name,child,children,COL_I_NULL));
        item->sortChildren(0);



    }

    sort(0);
    return items;

}

