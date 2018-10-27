#ifndef WIDGETIMAGEINFO_H
#define WIDGETIMAGEINFO_H

#include <QObject>
#include <QWidget>

class WidgetImageInfo : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetImageInfo(QWidget *parent = nullptr);
   virtual void paintEvent(QPaintEvent *event);
signals:

public slots:
    void setImage(QImage img){
          mImage48=img.scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation);
          mImage=img;
          update();
    }
    void setTitle(const QString &tit){mTitle=tit;}
    void setInfo(const QString &info){mInfo=info;}
    void setHorizontal(bool arg){mHorisontal=arg;}
private:

    QImage mImage;
     QImage mImage48;
    QString mTitle;
    QString mInfo;
    bool mHorisontal;

};

#endif // WIDGETIMAGEINFO_H
