#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H
#include "defines.h"
#include <QPainter>
#include <QObject>
#include <QStyledItemDelegate>

class ListItemDelegate : public QStyledItemDelegate
{
     Q_OBJECT
public:
    ListItemDelegate();

    //!
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const ;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const ;


signals:
  void  addAlbum(int);
  void  favoritAlbum(int,bool);
  void  playAlbum(int);
  void  changeImageAlbum(int);

private :
    //!  رسم الايقونة عند الوضع التفصيلي والقياسي
//    void paintDetailView(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!  رسم الايقونة في حالة وضع الايقونات
    void paintIconView(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!  ...جلب حالة الايقونة بين مفعلة وعادية و
    static QIcon::Mode iconModeFromState(QStyle::State state);

    //!   جلب الايقونة
 //   QIcon decoration(const QModelIndex &index) const;


    QMap<QString,QString> *mIconMap;
};

#endif // LISTITEMDELEGATE_H
