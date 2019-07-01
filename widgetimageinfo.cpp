/***************************************************************************
 *      Project created by QtCreator 2018-06-01T17:15:24                   *
 *                                                                         *
 *    goldfinch Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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

    Q_UNUSED(event)

    QPainter painter(this);
    QRect rect=this->rect();
    QFont font=this->font();
    font.setBold(true);

    QColor bgcolor=this->palette().base().color();
    QColor txtcolor=this->palette().windowText().color();
     QColor linecolor=this->palette().light().color();
   // QColor linecolor=this->palette().

    QTextOption txtOption;

    if(isLeftToRight())txtOption.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    else txtOption.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    txtOption.setWrapMode(QTextOption::NoWrap);
    QImage imgG=mImage.scaled(rect.height(),rect.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);

    if(mHorisontal){

      int _w=rect.width()-imgG.rect().width();
//        int _h=rect.height()/2;
        QFontMetrics fm(font);
 //       QRect textRect;

        if(isLeftToRight()){
            painter.drawImage(imgG.rect(),imgG,imgG.rect());
           // textRect=QRect(imgG.rect().width()+3,0,_w,_h);
        }else{
            painter.drawImage(QPoint(_w,0),imgG,imgG.rect());
           // textRect=QRect(0,0,_w-3,_h);
        }

        // TITLE
//        painter.setPen(txtcolor);
//        painter.setFont(font);
//        QString title=fm.elidedText(mTitle,Qt::ElideRight,_w);
//        painter.drawText(textRect,title, txtOption);
//        // INFO
//        textRect.moveTop(_h);
//        font.setBold(false);
//        painter.setFont(font);
//        QString info=fm.elidedText(mInfo,Qt::ElideRight,_w);
//        painter.drawText(textRect, info,txtOption);

        return;
    }

    font.setPointSize(font.pointSize()+2);
    painter.setFont(font);
    QFontMetrics fm(font);

    // IMAGE
    QImage img=mImage.scaled(rect.width(),rect.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    int i_x=(rect.width()-img.rect().width())/2;
    int i_y=(rect.height()-img.rect().height())/2;
    painter.drawImage(QPoint(i_x,i_y),img,img.rect());


    // RECT TEXT
    int _h=fm.height()+fm.leading()+10;

    QRect textRect=QRect(0,rect.height()-(_h*2),rect.width(),_h*2);

    painter.setOpacity(0.5);

    painter.fillRect(textRect,bgcolor.dark());

    painter.setOpacity(1.0);
      painter.setPen(linecolor);
    painter.drawLine(textRect.topLeft(),textRect.topRight());
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
