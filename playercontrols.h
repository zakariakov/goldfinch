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

QT_BEGIN_NAMESPACE
class QToolButton;
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
class QSlider;
class QLabel;
QT_END_NAMESPACE

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);

    QMediaPlayer::State state() const;

    bool isMuted() const;
    int volume() const;

public slots:
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);

     void setupIcons();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);

signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeRate(qreal rate);
   // void seek(int);
private slots:
    void playClicked();
    void onVolumeSliderValueChanged();
    void updateDurationInfo(qint64 currentInfo);

//    void setSliderPressed();
     void setSeeked(int val);

private:
  QToolButton *m_playButton = nullptr;

    QToolButton *m_nextButton = nullptr;
    QToolButton *m_previousButton = nullptr;
    QToolButton *m_muteButton = nullptr;
    QAbstractSlider *m_volumeSlider = nullptr;
    Slider *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;


    QIcon playIcon;
    QIcon pauseIcon;

    qint64 m_duration;
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;

// int m_pos2=0;
 //  int m_pos=0;
   bool m_playerMuted = false;
  // bool m_playerMuted2 = false;

};

#endif // PLAYERCONTROLS_H
