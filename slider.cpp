#include "slider.h"
#include <QWheelEvent>
#include <QDebug>
Slider::Slider()
{
    setOrientation(Qt::Horizontal);
}
void Slider::wheelEvent(QWheelEvent *event)
{
    int num = event->delta();
    int val=num<0 ? -10:10;
     setSliderPosition(value()+val);
  //  emit seekChanged(value()+val);
     QMetaObject::invokeMethod(parent()->parent(), "setSeek",Q_ARG(int,value()+val));
   qDebug()<<"Slider::wheelEvent:"<<value()+val;

    event->accept();
}

 void Slider::mouseReleaseEvent(QMouseEvent *ev)
 {

     int perx=ev->x()*100/width();
     int val=perx*maximum()/100;

     if(val<0)val=0;
     if(val>maximum())val=maximum();

     //    setValue(val);
     setSliderPosition(val);
     QMetaObject::invokeMethod(parent()->parent(), "setSeek",Q_ARG(int,val));
     setSliderDown(false);
     qDebug()<<" Slider::mouseReleaseEvent:"<<val;
 }

void Slider::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    setSliderDown(true);
}

 void Slider::mouseMoveEvent(QMouseEvent *ev)
 {
     setSliderDown(true);
     int perx=ev->x()*100/width();
     int val=perx*maximum()/100;
     if(val<0)val=0;
     if(val>maximum())val=maximum();
     //  setValue(val);
     setSliderPosition(val);
 }

 void Slider::keyPressEvent(QKeyEvent *ev)
 {
     int val=0;
     switch (ev->key()) {
     case Qt::Key_Left:
         val=value()+3;
         break;
     case Qt::Key_Right:
         val=value()-3;
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
     qDebug()<<"Slider::keyPressEvent"<<val;
     if(val==0)return;
     if(val<0)val=0;
     if(val>maximum())val=maximum();
     //
       QMetaObject::invokeMethod(parent()->parent(), "setSeek",Q_ARG(int,val));

 }
