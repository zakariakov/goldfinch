/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
    void seek(int);
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
   bool m_playerMuted = false;
    int m_pos=0;
};

#endif // PLAYERCONTROLS_H
