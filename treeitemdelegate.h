#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
class TreeItemDelegate : public QStyledItemDelegate
{
      Q_OBJECT
public:

    TreeItemDelegate();

    //!
   QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //!


signals:

};

#endif // TREEITEMDELEGATE_H
