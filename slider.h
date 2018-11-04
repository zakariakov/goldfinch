#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>
#include <QWidget>
#include <QSlider>
class Slider : public QSlider
{
       Q_OBJECT
public:
    Slider();
void wheelEvent(QWheelEvent *event);
 void mouseReleaseEvent(QMouseEvent *ev) ;
  void mousePressEvent(QMouseEvent *ev) ;
   void mouseMoveEvent(QMouseEvent *ev) ;
    void keyPressEvent(QKeyEvent *ev);
signals:
//void seekChanged(int );
};

#endif // SLIDER_H
