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

#include "actions.h"
#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QAudio>
#include <QTime>
#include <QLabel>
#include <QTextOption>
#include <QPainter>


//-----------------------------------------------------------------WidgetImg
void WidgetImg::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)

    QPainter painter(this);
    QRect rect=this->rect();

    QImage imgG=mImage.scaled(rect.height(),rect.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);

    QRect imgRec=imgG.rect();
    int _x=(rect.width()-imgRec.width())/2;
    int _y=(rect.height()-imgRec.height())/2;
    //painter.drawImage(imgG.rect(),imgG,rect);
    painter.drawImage(QPoint(_x,_y),imgG,imgRec);


}


//-----------------------------------------------------------------WidgetInfo
void WidgetInfo::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)



    QPainter painter(this);
    QRect rect=this->rect();
    QFont font=this->font();
    font.setBold(true);

    int _w=rect.width();
    int _h=rect.height()/2;
    QColor txtcolor=this->palette().windowText().color();

    QTextOption txtOption;

    if(isLeftToRight())txtOption.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    else txtOption.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    txtOption.setWrapMode(QTextOption::NoWrap);




        QFontMetrics fm(font);
        QRect textRect;

            textRect=QRect(0,0,_w,_h);


        // TITLE
        painter.setPen(txtcolor);
        painter.setFont(font);
        QString title=fm.elidedText(mTitle,Qt::ElideRight,_w);
        painter.drawText(textRect,title, txtOption);
        // INFO
        textRect.moveTop(_h);
        font.setBold(false);
        painter.setFont(font);
        QString info=fm.elidedText(mInfo,Qt::ElideRight,_w);
        painter.drawText(textRect, info,txtOption);






}

//-----------------------------------------------------------------
PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
{

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    m_WidgetInfo=new WidgetInfo;
    m_WidgetImg =new WidgetImg;
    m_WidgetImg->setMinimumSize(QSize(50,50));
    connect(this,&PlayerControls::imageChanged, m_WidgetImg,  &WidgetImg::setImage);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setMaximumWidth(100);
    m_volumeSlider->setMinimumWidth(80);

    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerControls::onVolumeSliderValueChanged);

    m_playButton = new QToolButton(this);
    m_playButton->setIconSize(QSize(16,16));
    m_playButton->setMinimumSize(QSize(30,30));
    m_playButton->setObjectName("PlayButton");
    m_playButton->setAutoRaise(true);
    m_playButton->setDefaultAction(ACtions::PlayPauseAct());

    m_nextButton = new QToolButton(this);
    m_nextButton->setIconSize(QSize(16,16));
    m_nextButton->setMinimumSize(QSize(30,30));
    m_nextButton->setObjectName("NextButton");
    m_nextButton->setAutoRaise(true);
    m_nextButton->setDefaultAction(ACtions::NextAct());

    m_previousButton = new QToolButton(this);
    m_previousButton->setIconSize(QSize(16,16));
    m_previousButton->setMinimumSize(QSize(30,30));
    m_previousButton->setObjectName("PrevButton");
    m_previousButton->setAutoRaise(true);
    m_previousButton->setDefaultAction(ACtions::PrevAct());

    m_muteButton = new QToolButton(this);
    m_muteButton->setObjectName("VolumeButton");
    m_muteButton->setIconSize(QSize(16,16));
    m_muteButton->setCheckable(true);
    m_muteButton->setAutoRaise(true);
    m_muteButton->setDefaultAction(ACtions::muteUnmuteAct());

    m_slider = new Slider;

    m_labelDuration= new QLabel(this);

    QBoxLayout *vlayoutInfo = new QVBoxLayout;
    vlayoutInfo->addWidget(m_WidgetInfo);


    QBoxLayout *layoutDuration = new QHBoxLayout;
    QBoxLayout *hlayout = new QHBoxLayout;
    QBoxLayout *vlayout = new QVBoxLayout;
    QBoxLayout *layout = new QHBoxLayout;
    layoutDuration->setMargin(0);
    hlayout->setMargin(3);
    vlayout->setMargin(3);
    layout->setMargin(3);

    layoutDuration->setSpacing(0);
    hlayout->setSpacing(0);
    vlayout->setSpacing(0);
   layout->setSpacing(3);

    hlayout->addLayout(vlayoutInfo);
    //   hlayout->addStretch();

    hlayout->addWidget(m_previousButton);
    hlayout->addWidget(m_playButton);
    hlayout->addWidget(m_nextButton);

    //hlayout->addStretch();
    hlayout->addSpacing(10);
    hlayout->addWidget(m_muteButton);
    hlayout->addWidget(m_volumeSlider);



    layoutDuration->addWidget(m_slider);
    layoutDuration->addWidget(m_labelDuration);


    vlayout->addLayout(hlayout);
    layout->addSpacing(9);
    vlayout->addLayout(layoutDuration);

    layout->addWidget(m_WidgetImg);
    layout->addSpacing(3);
    layout->addLayout(vlayout);
    setLayout(layout);
    setupIcons();

}

//----------------------------------------
void PlayerControls::setupIcons()
{

    playIcon=Tumb::icon(I_PLAY);
    pauseIcon=Tumb::icon(I_PAUSE);
    m_nextButton->setIcon(Tumb::icon(I_NEXT));
    m_previousButton->setIcon(Tumb::icon(I_PREV));
    m_muteButton->setIcon(Tumb::icon(I_VOLUME));

}

void PlayerControls::setLabTitle(const QString title, const QString info)
{

    m_WidgetInfo->setTitle(title,info);
}



void PlayerControls::setMutedChanged(bool mute)
{
   if(mute)
        m_muteButton->setIcon(Tumb::icon(I_VOLUME_OF));
   else {
        m_muteButton->setIcon(Tumb::icon(I_VOLUME));
   }
}
QMediaPlayer::State PlayerControls::state() const
{
    return m_playerState;
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

}


