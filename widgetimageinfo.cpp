#include "widgetimageinfo.h"
#include <QPainter>
#include <QFontMetrics>
#include <QTextOption>
WidgetImageInfo::WidgetImageInfo(QWidget *parent) : QWidget(parent)
{

mHorisontal=true;
setToolTip(mTitle);

}

void WidgetImageInfo::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);

    QPainter painter(this);
    QRect rect=this->rect();
    QFont font=this->font();
    font.setBold(true);

    QColor bgcolor=this->palette().base().color();
    QColor txtcolor=this->palette().windowText().color();
    //  QColor shadowcolor=this->palette().highlight().color();
    QTextOption txtOption;

    if(isLeftToRight())txtOption.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    else txtOption.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    txtOption.setWrapMode(QTextOption::NoWrap);

    if(mHorisontal){

        int _w=rect.width()-mImage48.rect().width();
        int _h=rect.height()/2;
        QFontMetrics fm(font);
        QRect textRect;

        if(isLeftToRight()){
            painter.drawImage(mImage48.rect(),mImage48,mImage48.rect());
            textRect=QRect(mImage48.rect().width()+3,0,_w,_h);
        }else{
            painter.drawImage(QPoint(_w,0),mImage48,mImage48.rect());
            textRect=QRect(0,0,_w-3,_h);
        }

        // TITLE
        painter.setPen(txtcolor);
        painter.setFont(font);
        QString title=fm.elidedText(mTitle,Qt::ElideRight,_w);
        painter.drawText(textRect,title, txtOption);
        // INFO
        textRect.moveTop(_h);
        font.setBold(false);
        painter.setFont(font);
        QString info=fm.elidedText(mInfo,Qt::ElideRight,_w);
        painter.drawText(textRect, info,txtOption);

        return;
    }

    font.setPointSize(font.pointSize()+2);
    painter.setFont(font);
    QFontMetrics fm(font);

    // IMAGE
    painter.drawImage(rect,mImage,mImage.rect());

    // RECT TEXT
    int _h=fm.height()+fm.leading()+10;

    QRect textRect=QRect(0,rect.height()-(_h*2),rect.width(),_h*2);

    painter.setOpacity(0.5);
    painter.fillRect(textRect,bgcolor);

    painter.setOpacity(1.0);
    // TEXT TITLE
    painter.setPen(bgcolor);

    textRect.adjust(2,2,0,-_h);
    QString title=fm.elidedText(mTitle,Qt::ElideRight,textRect.width()-20);
    painter.drawText(textRect, Qt::AlignCenter, title);

    textRect.adjust(2,2,0,0);
    painter.setPen(txtcolor);
    painter.drawText(textRect, Qt::AlignCenter, title);

    // TEXT INFO
    font.setPointSize(font.pointSize()-2);
    painter.setFont(font);
    painter.setPen(bgcolor);

    textRect.moveTop(textRect.top()+_h);
    QString info=fm.elidedText(mInfo,Qt::ElideRight,textRect.width());
    painter.drawText(textRect, Qt::AlignCenter, info);

    textRect.adjust(2,2,0,0);
    painter.setPen(txtcolor);
    painter.drawText(textRect, Qt::AlignCenter, info);


}
