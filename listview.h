#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QObject>
#include <QListView>

class ListView : public QListView
{
    Q_OBJECT
public:
    ListView();
    QModelIndex curHighlilghtedIndex(){
        if(mOldIndex.isValid())
        return mOldIndex;
        return QModelIndex();
    }
private slots:
 //   Q_SIGNALS:
  void  onItemEntered(const QModelIndex &index);
  void selectionHasChanged();

private:
  QModelIndex mOldIndex;
  //!
  QItemSelectionModel *listSelectionModel;
};

#endif // LISTVIEW_H
