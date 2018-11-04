#include "listitemdelegate.h"
#include "tumb.h"
#include "listeditor.h"
#include<QTextOption>
#include<QFileInfo>
#include<QFile>
#include<QImageReader>

//#include<QDebug>
#include<QDir>
ListItemDelegate::ListItemDelegate()
{
    mIconMap=new QMap<QString,QString> ;
}

void ListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;

    // QStyleOptionViewItem opt = option;

    //اذا لم لم يكن العنصر يحمل ايقونة
    //سيتم حمل الاعدادات الافتراضية
    if(index.column()!=0){

        QStyledItemDelegate::paint(painter,option,index);
        return;
    }

    //منظور الايقونات (ICON view)-----------------------------------------------
    if(option.decorationPosition == QStyleOptionViewItem::Top ||
            option.decorationPosition == QStyleOptionViewItem::Bottom) {

        paintIconView(painter,option,index);
    }
    //! منظر تفصيلي (Detail View)-----------------------------------------------
    else{
        //  paintDetailView(painter,option,index);
    }

}
QIcon decoration(const QModelIndex &index){

    int col=index.data(USER_ID).toInt();
    QString title=index.data(USER_TITLE).toString();

    if(col==COL_I_ALBUM/*||col==CAT_ALBUMRATED*/){
        QString imgPath=index.data(USER_IMGPATH).toString();
        return  Tumb::iconAlbum(title,imgPath);
    }else if(col==COL_I_ARTIST){
        return (Tumb::iconnArtist(title));
    }
    return (Tumb::iconnArtist(title));
}
//______________________________________________________________________________
void ListItemDelegate::paintIconView(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    painter->save();

    QSize itemSize=QSize(option.rect.width(),option.rect.height());

    // المتغيرات--------------------------------------
    QTextOption textOption;
    QString txt=index.data(USER_TITLE).toString();
    QString txtParent=index.data(USER_PARENT_NAME).toString();
    int id=index.data(USER_ID).toInt();

    QFontMetrics fm1=option.fontMetrics;

    QIcon ico=decoration(index);

    QPixmap pixmap = ico.pixmap(option.decorationSize,iconModeFromState(option.state));

    //  painter->save();
    painter->setClipRect(option.rect);

    //رسم التحديد
    //        if((option.state & QStyle::State_Selected) )
    //            painter->fillRect(option.rect, option.palette.highlight());

    // draw the icon
    QPoint iconPos(option.rect.x() + (option.rect.width() - option.decorationSize.width()) / 2,
                   option.rect.y() +(option.decorationSize.height() - pixmap.height()) / 2+ D_MARGINS);
    //   QPoint iconPos(option.rect.x()+ D_MARGINS , option.rect.y() + D_MARGINS);

    // in case the pixmap is smaller than the requested size
    QSize margin = ((option.decorationSize - pixmap.size()) / 2).expandedTo(QSize(0, 0));

    QRect rectborder=option.rect;

    if(id==COL_I_ALBUM){
        rectborder.adjust(0,0,-1,-1);
        painter->setOpacity(0.3);
        painter->setPen(option.palette.highlight().color());
        painter->drawRoundedRect(rectborder,qreal(2.0),qreal(2.0));
        if(option.state & QStyle::State_Selected){
            rectborder.adjust(1,1,0,0);
            painter->fillRect(rectborder,option.palette.highlight());
        }
    }
    painter->setOpacity(1.0);

    //------------------------------------------------------------

    painter->drawPixmap(iconPos + QPoint(margin.width(), 1), pixmap);

    //------------------------  رسم النص  --------------------------
    //مربع النص
    QRect textRect(option.rect.x() + (option.rect.width() - itemSize.width()) / 2,
                   option.rect.y() + (D_MARGINS*2) + option.decorationSize.height(),
                   itemSize.width(),
                   itemSize.height() - option.decorationSize.height());


    textOption.setAlignment(Qt::AlignHCenter);

    //رسم التحديد على مربع النص
    if((option.state & QStyle::State_Selected) )
        painter->fillRect(textRect, option.palette.highlight());

    //لون النص عند التحديد او الوضع العادي
    if(option.state & QStyle::State_Selected)
        painter->setPen(option.palette.color( QPalette::HighlightedText));
    else
        painter->setPen(option.palette.color( QPalette::Text));



    painter->setOpacity(0.2);
    painter->fillRect(textRect, option.palette.highlight());
    painter->setOpacity(1.0);
    // qDebug()<<option.fontMetrics.height()+option.fontMetrics.leading();;
    textRect.adjust(1,D_MARGINS,-1,-D_MARGINS);
    textRect.setHeight(fm1.height()+(fm1.leading()*2)+D_MARGINS);
    txt=fm1.elidedText(txt,Qt::ElideRight,textRect.width());
    //   painter->save();
    QFont font=option.font;
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(textRect,txt,textOption);

    //  painter->restore();
    int h=textRect.height();
    //  qDebug()<<textRect<<h;
    font.setBold(false);
    font.setPointSize(font.pointSize()-2);
    font.setItalic(true);
    painter->setFont(font);
    txtParent=fm1.elidedText(txtParent,Qt::ElideRight,textRect.width());
    textRect.moveTop(textRect.top()+h);

    painter->setPen(option.palette.color(QPalette::Disabled,QPalette::WindowText));
    painter->drawText(textRect,txtParent,textOption);
    painter->restore();


}


int sizeWidh(int size){
    int defW=70;
    if(size<=64)defW=50;
    else if(size<=96)defW=D_MARGINS*2;
    else if(size<128)defW=D_MARGINS*2;
    else if(size<=256)defW=D_MARGINS*2;

    return defW;
}

//______________________________________________________________________________
QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    // QSize s= QAbstractItemDelegate::sizeHint(option,index);
    if(!index.isValid()) return  QSize();


    //ICON VIEW MODE
    if(option.decorationPosition == QStyleOptionViewItem::Top ||
            option.decorationPosition == QStyleOptionViewItem::Bottom)
    {

        QString txt=index.data(Qt::EditRole).toString();


        int size=option.decorationSize.width();
        int defW=sizeWidh(size);

        //طول الحاوي للنص
        int _w=option.decorationSize.width()+(defW);
        int line;
        //         if(isModernMode){
        line=option.fontMetrics.height()+option.fontMetrics.leading();
        line+=D_MARGINS*2;

        // int _h=option.decorationSize.height()+(D_MARGINS*2)+(line*2);
        int _h=option.decorationSize.height()+(D_MARGINS)+(line*2);


        if(size<94){
            //  qDebug()<<"hint"<<_w<<_h;
            return  QSize(_h,_h);
        }else{
            //   qDebug()<<"hint"<<_w<<_h;
            return  QSize(_w,_h);
        }
    }

    return  QSize();
}

//______________________________________________________________________________
QIcon::Mode ListItemDelegate::iconModeFromState(const QStyle::State state)
{

    if(state & QStyle::State_Enabled) {
        if((state & QStyle::State_Selected) )
            return  QIcon::Selected;

        if((state & QStyle::State_MouseOver) )
            return  QIcon::Active;

        return QIcon::Normal;
    }

    return QIcon::Disabled;

}

//______________________________________________________________________________ //IDITOR
QWidget *ListItemDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const

{

    //! *----------------------* IDITOR *---------------------------*//
    if (index.data().canConvert<Iditoring>()) {
        ListEditor *editor = new ListEditor(index,parent);
        //  editor->setIndex(index);
        connect(editor,SIGNAL(addAlbum(int)),this,SIGNAL(addAlbum(int)));
        connect(editor,SIGNAL(playAlbum(int)),this,SIGNAL(playAlbum(int)));
        connect(editor,SIGNAL(changeImageAlbum(int)),this,SIGNAL(changeImageAlbum(int)));

        connect(editor,SIGNAL(favoritAlbum(int,bool)),this,SIGNAL(favoritAlbum(int,bool)));

        return editor;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

}

void ListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    //! *----------------------* IDITOR *---------------------------*//
    if (index.data().canConvert<Iditoring>()) {
        ListEditor *starEditor = qobject_cast<ListEditor *>(editor);
        model->setData(index, QVariant::fromValue(starEditor->iditoring()));
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }

}


