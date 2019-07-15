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

#include "slider.h"
#include <QWheelEvent>
#include <QDebug>
#include <QApplication>
//------------------------------------------------------
QString myStyle()
{
    QString mStyle=
            " QSlider::groove:horizontal {"
            " height: 8px;"
            " background: transparent;"
            " margin: 6px 0px;"
            " }"

            " QSlider::handle:horizontal {"
            "background:transparent;"/* لاتغير هذه */
            " width: 14px;"
            " margin: -3px 0px; "
            " border-radius: 7px;"
            "}";

    if(QApplication::isLeftToRight()){
        mStyle+=   " QSlider::add-page:horizontal {"
                   " background: palette(base ) ;"
                   " margin: 8px 0; "
                   " border-radius: 2px;"
                   " }"

                   " QSlider::sub-page:horizontal {"
                   " background: palette(Highlight );"
                   " margin: 8px 0; "
                   " border-radius: 2px;"
                   " }";
    }else{
        mStyle+=   " QSlider::add-page:horizontal {"
                   " background: palette(Highlight ) ;"
                   " margin: 8px 0; "
                   "border-radius: 2px;"
                   " }"

                   " QSlider::sub-page:horizontal {"
                   " background: palette(base );"
                   " margin: 8px 0; "
                   " border-radius: 2px;"
                   " }";
    }
    return mStyle;

}

Slider::Slider()
{
    setOrientation(Qt::Horizontal);
    setContentsMargins(0,0,0,0);

    setStyleSheet(myStyle());
}
void Slider::wheelEvent(QWheelEvent *event)
{
    int num = event->delta();
    int val=num<0 ? -pageStep():pageStep();
     setSliderPosition(value()+val);
  //  emit seekChanged(value()+val);
     QMetaObject::invokeMethod(parent()->parent(), "setSeek",Q_ARG(int,value()+val));
   //qDebug()<<"Slider::wheelEvent:"<<value()+val;

    event->accept();
}

 void Slider::mouseReleaseEvent(QMouseEvent *ev)
 {

     int perx=ev->x()*100/width();
     int val=perx*maximum()/100;

     if(val<0)val=0;
     if(val>maximum())val=maximum();

     if(isRightToLeft())
         val=maximum()-val;
        setValue(val);
    //  setSliderPosition(val);
     QMetaObject::invokeMethod(parent()->parent(), "setSeek",Q_ARG(int,val));
     setSliderDown(false);
     //qDebug()<<" Slider::mouseReleaseEvent:"<<val;
 }

void Slider::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    setSliderDown(true);
}

 void Slider::mouseMoveEvent(QMouseEvent *ev)
 {
     setSliderDown(true);

     int perx=ev->x()*100/width();

     int val=perx*maximum()/100;

     if(val<0)val=0;
     if(val>maximum())val=maximum();

     if(isRightToLeft())
         val=maximum()-val;

    // qDebug()<<"Slider::mouseMoveEvent:"<<val;
   //  setValue(val);
      setSliderPosition(val);

 }

 void Slider::keyPressEvent(QKeyEvent *ev)
 {
     int val=0;
     switch (ev->key()) {
     case Qt::Key_Left:
         val=value()+singleStep();
         break;
     case Qt::Key_Right:
         val=value()-singleStep();
         break;
     case Qt::Key_Up:
         val=value()+pageStep();
         break;
     case Qt::Key_Down:
         val=value()-pageStep();
         break;
     case Qt::Key_PageUp:
         val=value()+(pageStep()*2);
         break;
     case Qt::Key_PageDown:
         val=value()-(pageStep()*2);
         break;
     }

     if(val==0)return;
     if(val<0)val=0;
     if(val>maximum())val=maximum();

     // qDebug()<<"Slider::keyPressEvent:"<<val;
     //
       QMetaObject::invokeMethod(parent()->parent(), "setSeek",Q_ARG(int,val));

 }

//------------------------------------------------------
void Slider::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
   // setStyleSheet(myStyle(true));
setStyleSheet(styleSheet().replace("background:transparent;","background:palette(Highlight);"));
}

//------------------------------------------------------
void Slider::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
   // setStyleSheet(myStyle(false));
setStyleSheet(styleSheet().replace("background:palette(Highlight);","background:transparent;"));
}

