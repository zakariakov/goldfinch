#ifndef MYCONTENTMODEL_H
#define MYCONTENTMODEL_H
//#include "defines.h"
#include "database.h"
#include <QStandardItemModel>
#include <QApplication>
#include <QDir>



class MyContentModel : public QStandardItemModel
{
      Q_OBJECT
public:
    explicit MyContentModel( QObject *parent = nullptr);

 QIcon icon(const QModelIndex &index){return item(index.row())->icon();}
protected:

 //   QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  //  int columnCount(const QModelIndex &) const{return 5;}

private:


 // DataBase *mDataBase;
  //  int  mediaMeyaData;
//    int  curGroup;
//     bool isGroup;

public slots:

   void chargeCategory(int root, int child=COL_I_NULL, int children=COL_I_NULL);
 QList<QStandardItem *> addChilds(QString prentName, int parentColumn, int child, int children=COL_I_NULL);

private slots:
//void chargeFavoritedAudio();
void chargeFavoritedAlbum();


};

#endif // MYCONTENTMODEL_H
