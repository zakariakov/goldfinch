#include "treeitemdelegate.h"
#include "listeditor.h"
#include "tumb.h"
#include <QPainter>
TreeItemDelegate::TreeItemDelegate()
{

}

void TreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;


    if(  option.state & QStyle::State_MouseOver )  {

        painter->setOpacity(0.3);
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setOpacity(1.0);
    }

    QStyledItemDelegate::paint(painter,option,index);
}

// ______________________________________________________________________________
QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(!index.isValid()) return  QSize();

    return QStyledItemDelegate::sizeHint(option,index);
}


