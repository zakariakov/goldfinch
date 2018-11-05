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


