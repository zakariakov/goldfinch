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
/*
     return;
    QString txt=QString();
    QIcon icon=QIcon();
    QPixmap pixmap =QPixmap();
    if(index.column()==0){
       //icon=Tumb::icon(I_ADD);
      icon=  option.icon;
    }
   else
    if(index.column()==0||index.column()==1){
        // bool favo =index.data(USER_FAVO).toInt();
        // icon=!favo ? Tumb::icon(I_START):Tumb::icon(I_STARTED);
        icon=option.icon;
        pixmap = icon.pixmap(option.decorationSize);
    }
    else{
        txt=index.data().toString();
    }

    //رسم التحديد على كامل المربع
    if((option.state & QStyle::State_Selected) ) {

        painter->fillRect(option.rect, option.palette.highlight());
    }

    //رسم التحديد على كامل المربع
    if((option.state & QStyle::State_MouseOver) ) {
        painter->setOpacity(0.5);
        painter->fillRect(option.rect, option.palette.highlight());
         painter->setOpacity(1.0);
    }

    if(index.column()==0||index.column()==1){
        QPoint iconPos=QPoint(option.rect.x() + (option.rect.width() - option.decorationSize.width()),
                              option.rect.y() +(option.rect.height()-pixmap.height())/2);

        //رسم الايقونة
        painter->drawPixmap(iconPos , pixmap);
    }else{
     QRect   rect=QRect(option.rect.x(),
                   option.rect.y(),
                   option.rect.width()-option.decorationSize.width(),
                   option.rect.height());

     QFontMetrics fm1=option.fontMetrics;
     txt=fm1.elidedText(txt,Qt::ElideRight,rect.width());
     painter->drawText(rect,txt);
    }



  //  QStyledItemDelegate::paint(painter,option,index);
  */
}

// ______________________________________________________________________________
QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(!index.isValid()) return  QSize();

    return QStyledItemDelegate::sizeHint(option,index);
}


