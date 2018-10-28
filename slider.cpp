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
emit seekChanged(val);
  qDebug()<<num<<val;



     event->accept();
 }
