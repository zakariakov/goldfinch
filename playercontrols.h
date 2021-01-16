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

#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H
#include "slider.h"
#include <QMediaPlayer>

#include <QWidget>
#include <QIcon>
#include <QDebug>
#include <QBoxLayout>
#include <QGraphicsBlurEffect>
#include <QLabel>
QT_BEGIN_NAMESPACE
class QToolButton;
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
class QSlider;
class QLabel;

QT_END_NAMESPACE

//---------------------------------- WidgetInfo ---------------------------------
class WidgetImg : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetImg(){
        setAttribute(Qt::WA_PaintUnclipped,false);


    }
    virtual void paintEvent(QPaintEvent *event);
signals:

public slots:
    void setImage(QImage img){
        mImage=img;
        update();
    }

private:

    QImage mImage;


};
//---------------------------------- WidgetInfo ---------------------------------
class WidgetInfo : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetInfo(){   setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);}
    virtual void paintEvent(QPaintEvent *event);
signals:

public slots:

    void setTitle(const QString &tit,const QString &info){
        mTitle=tit;mInfo=info;update();
        setToolTip(mTitle+"\n"+mInfo);
        update();
    }

private:
    QString mTitle;
    QString mInfo;
};

//---------------------------------- PlayerControls ---------------------------------
class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event)
    {
        Q_UNUSED(event)
        m_WidgetImg->setMinimumWidth(this->sizeHint().height()-5);
        qDebug()<<this->sizeHint().height()-5;
    }
    QMediaPlayer::State state() const;
    virtual void paintEvent(QPaintEvent *event);
    bool isMuted() const;
    int volume() const;
    void togglePreview(bool minPrev);
public slots:

    //    void setState(QMediaPlayer::State state);
    void setSeekSliderValueChanged(int val);
    void setVolume(int volume);
    void setLabTitle(const QString title,const QString info);
    void setupIcons();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void setMutedChanged(bool mute);
signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeRate(qreal rate);
    void seek(int);
    void imageChanged(QImage img);
private slots:
    void setImage(QImage img);
    //  void playClicked();
    void onVolumeSliderValueChanged();
    void updateDurationInfo(qint64 currentInfo);

    //    void setSliderPressed();
 //   void setSeeked(int val);

private:
    WidgetInfo *m_WidgetInfo;
    WidgetImg *m_WidgetImg;
    QWidget *widgetMini;
    QWidget *widgetButtons;
    QToolButton *m_playButton = nullptr;
    QToolButton *m_nextButton = nullptr;
    QToolButton *m_previousButton = nullptr;
    QToolButton *m_muteButton = nullptr;
    QToolButton *m_toggleButton=nullptr;
    QAbstractSlider *m_volumeSlider = nullptr;
    Slider *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;
    QLabel *m_labelCurrentTime = nullptr;
    //QLabel *m_labelTitle=nullptr;
    QIcon playIcon;
    QIcon pauseIcon;
    QImage mImage;
    qint64 m_duration;
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;
    QBoxLayout *HlayoutDuration ;
    QBoxLayout *hlayout ;
    QBoxLayout *vMainlayout ;
    QBoxLayout *hMainlayout;
    QBoxLayout *vlayoutDuration;

    QBoxLayout *vMinilayout;

    bool m_playerMuted = false;

    bool mMinPreview=false;

};

#endif // PLAYERCONTROLS_H
