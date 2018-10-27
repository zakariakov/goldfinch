#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H
#include "setting.h"
#include "database.h"
#include <QStandardItemModel>
#include <QObject>
#include <QUrl>
class MyListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    MyListModel(Setting *setting, DataBase *data, QObject *parent = nullptr);

    void chargeCategory(QString curentName, int curent, int child, int pId, QString pName);
    void chargeAudios(QString name, int colm,
                      QString pName=QString(), int pColm=0,
                      QString pPname=QString(), int pPcolm=0);
void chargeFavoritedAlbum();

  QIcon icon(const QModelIndex &index){return item(index.row())->icon();}
protected:

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &) const{return 6;}

private:
    Setting *mSetting;
    DataBase *mDataBase;
 //   QList<QStandardItem*>listItem;
};

#endif // MYLISTMODEL_H
