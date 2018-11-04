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

#include "playercontrols.h"
#include "tumb.h"
#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QAudio>
#include <QTime>
#include <QLabel>
PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
{

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setMaximumWidth(100);
    m_volumeSlider->setVisible(false);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerControls::onVolumeSliderValueChanged);

    m_playButton = new QToolButton(this);
    m_playButton->setIconSize(QSize(16,16));
    m_playButton->setMinimumSize(QSize(30,30));
    m_playButton->setObjectName("PlayButton");
    m_playButton->setAutoRaise(true);
    m_playButton->setShortcut(Qt::Key_Space);
    m_playButton->setToolTip(tr("Play/Pause \n")+m_playButton->shortcut().toString());
    //  m_playButton->animateClick(500);
    connect(m_playButton, &QAbstractButton::clicked, this, &PlayerControls::playClicked);

    m_nextButton = new QToolButton(this);
    m_nextButton->setIconSize(QSize(16,16));
    m_nextButton->setMinimumSize(QSize(30,30));
    m_nextButton->setObjectName("NextButton");
    m_nextButton->setAutoRaise(true);
    m_nextButton->setShortcut(QKeySequence("Alt+Left"));
    m_nextButton->setToolTip(tr("Next Song \n")+m_nextButton->shortcut().toString());
    connect(m_nextButton, &QAbstractButton::clicked, this, &PlayerControls::next);

    m_previousButton = new QToolButton(this);
    m_previousButton->setIconSize(QSize(16,16));
    m_previousButton->setMinimumSize(QSize(30,30));
    m_previousButton->setObjectName("PrevButton");
    m_previousButton->setAutoRaise(true);
    m_previousButton->setShortcut(QKeySequence("Alt+Right"));
    m_previousButton->setToolTip(tr("Previous Song \n")+m_previousButton->shortcut().toString());
    connect(m_previousButton, &QAbstractButton::clicked, this, &PlayerControls::previous);

    m_muteButton = new QToolButton(this);
    m_muteButton->setObjectName("VolumeButton");
    m_muteButton->setIconSize(QSize(16,16));
    m_muteButton->setCheckable(true);
    m_muteButton->setAutoRaise(true);
    connect(m_muteButton, &QAbstractButton::toggled, m_volumeSlider,&QSlider::setVisible);

    m_slider = new Slider;

    m_labelDuration = new QLabel(this);
 //connect(m_slider, &QSlider::sliderMoved, this, &PlayerControls::seek);
  //   connect(m_slider, &Slider::seekChanged, this, &PlayerControls::setSeeked);
  //  connect(m_slider, &Slider::valueChanged, this, &PlayerControls::setSeeked);

    QBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(m_previousButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_nextButton);
    layout->addSpacing(5);
    layout->addWidget(m_slider);
    layout->addWidget(m_labelDuration);
    layout->addSpacing(3);
    layout->addWidget(m_muteButton);
    layout->addWidget(m_volumeSlider);

    setLayout(layout);
    setupIcons();

}

//----------------------------------------
void PlayerControls::setupIcons()
{
    // QColor col=this->palette().text().color();
    playIcon=Tumb::icon(I_PLAY);
    pauseIcon=Tumb::icon(I_PAUSE);
    m_playButton->setIcon(playIcon);

    m_nextButton->setIcon(Tumb::icon(I_NEXT));
    m_previousButton->setIcon(Tumb::icon(I_PREV));
    m_muteButton->setIcon(Tumb::icon(I_VOLUME));

}

QMediaPlayer::State PlayerControls::state() const
{
    return m_playerState;
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != m_playerState) {
        m_playerState = state;
        switch (state) {
        case QMediaPlayer::StoppedState: m_playButton->setIcon(playIcon);  break;
        case QMediaPlayer::PlayingState: m_playButton->setIcon(pauseIcon); break;
        case QMediaPlayer::PausedState:  m_playButton->setIcon(playIcon);  break;
        }
    }
}


int PlayerControls::volume() const
{
    qreal linearVolume =  QAudio::convertVolume(m_volumeSlider->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

void PlayerControls::setVolume(int volume)
{
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);

    m_volumeSlider->setValue(qRound(logarithmicVolume * 100));
}

bool PlayerControls::isMuted() const
{
    return m_playerMuted;
}

void PlayerControls::playClicked()
{
    switch (m_playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void PlayerControls::onVolumeSliderValueChanged()
{
    emit changeVolume(volume());
}


void PlayerControls::durationChanged(qint64 duration)
{

    m_duration = duration / 1000;
    m_slider->setMaximum(QVariant(m_duration).toInt());
}

void PlayerControls::positionChanged(qint64 progress)
{
    QVariant value=progress / 1000;
 //   m_pos=value.toInt();
    if (!m_slider->isSliderDown())
        m_slider->setValue(value.toInt());

    updateDurationInfo(value.toLongLong());
}


void PlayerControls::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
                        m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    m_labelDuration->setText(tStr);
}

void PlayerControls:: setSeeked(int val)
{

    int value=/*m_pos+*/val;
    if(value>m_duration)value=QVariant(m_duration).toInt();
    if(value<0)value=0;
 QMetaObject::invokeMethod(parent(), "setSeek",Q_ARG(int,value));
 //   emit seek(value);
}


