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

signals:
void seekChanged(int );
};

#endif // SLIDER_H
