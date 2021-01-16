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
#include <QApplication>
#include <QGraphicsBlurEffect>
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
    font.setPointSize(this->font().pointSize()+3);
    QFontMetrics fm(font);
    int minH=fm.height()*2;
    int _w=rect.width();
    setMinimumHeight(minH);

    int _h=minH/2;
    QColor txtcolor=this->palette().windowText().color();

    QTextOption txtOption;

    txtOption.setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    txtOption.setWrapMode(QTextOption::NoWrap);


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
    font.setPointSize(this->font().pointSize());
    painter.setFont(font);
    QString info=fm.elidedText(mInfo,Qt::ElideRight,_w);
    painter.drawText(textRect, info,txtOption);


}
//----------------------------------------------------------------
//                        PlayerControls
//-----------------------------------------------------------------


PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
{

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    m_WidgetInfo=new WidgetInfo;
    m_WidgetImg =new WidgetImg;
    widgetMini=new QWidget;
    widgetMini->setVisible(false);
    m_WidgetImg->setMinimumSize(QSize(50,50));

    connect(this,&PlayerControls::imageChanged, m_WidgetImg,  &WidgetImg::setImage);
    connect(this,&PlayerControls::imageChanged, this,  &PlayerControls::setImage);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setObjectName("VolumeSlider");
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setMaximumWidth(100);
    m_volumeSlider->setMinimumWidth(80);
    // m_volumeSlider->setStyleSheet(myStyleSheet());
    m_volumeSlider->setCursor(Qt::PointingHandCursor);
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

    m_toggleButton = new QToolButton(this);
    m_toggleButton->setObjectName("ToggleButton");
    m_toggleButton->setIconSize(QSize(16,16));
    m_toggleButton->setCheckable(true);
    m_toggleButton->setAutoRaise(true);
    m_toggleButton->setDefaultAction(ACtions::swichMimiPlayerAct());


    m_slider = new Slider;
    // m_slider->setStyleSheet(mStyle);
    connect(m_slider, &Slider::seekChanged, this, &PlayerControls::setSeekSliderValueChanged);

    widgetButtons=new QWidget;
    widgetButtons->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    QHBoxLayout *hbuttonsLayout=new QHBoxLayout;
    hbuttonsLayout->setMargin(0);
    hbuttonsLayout->setSpacing(6);
    widgetButtons->setLayout(hbuttonsLayout);
    hbuttonsLayout->addWidget(m_previousButton);
    hbuttonsLayout->addWidget(m_playButton);
    hbuttonsLayout->addWidget(m_nextButton);

    m_labelDuration= new QLabel(this);
    m_labelCurrentTime= new QLabel(this);
    HlayoutDuration = new QHBoxLayout;
    vlayoutDuration = new QVBoxLayout;

    hlayout = new QHBoxLayout;
    vMainlayout = new QVBoxLayout;
    hMainlayout = new QHBoxLayout;

    vMinilayout=new QVBoxLayout;
//    horizontalSpacer=new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    horizontalSpacer2=new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

    widgetMini->setLayout(vMinilayout);
    widgetMini->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    setLayout(hMainlayout);
    setupIcons();
//setAutoFillBackground(true);
//  setAttribute(Qt::WA_TranslucentBackground);
//setAttribute(Qt::WA_OpaquePaintEvent);


//QGraphicsBlurEffect *blurEffect=new QGraphicsBlurEffect(this);
// blurEffect->setBlurRadius(7);
//setGraphicsEffect(blurEffect);

}

QImage blurred(const QImage& image, const QRect& rect, int radius, bool alphaOnly = false)
{
    int tab[] = { 14, 10, 8, 6, 5, 5, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 };
    int alpha = (radius < 1)  ? 16 : (radius > 17) ? 1 : tab[radius-1];

    QImage result = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    int r1 = rect.top();
    int r2 = rect.bottom();
    int c1 = rect.left();
    int c2 = rect.right();

    int bpl = result.bytesPerLine();
    int rgba[4];
    unsigned char* p;

    int i1 = 0;
    int i2 = 3;

    if (alphaOnly)
        i1 = i2 = (QSysInfo::ByteOrder == QSysInfo::BigEndian ? 0 : 3);

    for (int col = c1; col <= c2; col++) {
        p = result.scanLine(r1) + col * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p += bpl;
        for (int j = r1; j < r2; j++, p += bpl)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++) {
        p = result.scanLine(row) + c1 * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p += 4;
        for (int j = c1; j < c2; j++, p += 4)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int col = c1; col <= c2; col++) {
        p = result.scanLine(r2) + col * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p -= bpl;
        for (int j = r1; j < r2; j++, p -= bpl)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++) {
        p = result.scanLine(row) + c2 * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p -= 4;
        for (int j = c1; j < c2; j++, p -= 4)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    return result;
}

void PlayerControls::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)

    QPainter painter(this);
    if(!mMinPreview)
    {
        painter.restore();
        return;
    }

    painter.setOpacity(0.45);

    painter.drawImage(rect(),mImage,mImage.rect());

}

void PlayerControls::setImage(QImage img)
{
    mImage=img.scaled(256,256,Qt::IgnoreAspectRatio,Qt:: SmoothTransformation);
         mImage =  blurred(mImage,mImage.rect(),50,false);
         update();

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

void PlayerControls::togglePreview(bool minPrev)
{
    mMinPreview=minPrev;
    HlayoutDuration->setMargin(0);
    hlayout->setMargin(0);
    vMainlayout->setMargin(0);
    vlayoutDuration->setMargin(0);


    HlayoutDuration->setSpacing(5);
    vMainlayout->setSpacing(0);
    hMainlayout->setSpacing(6);
    vlayoutDuration->setSpacing(0);
    hlayout->setSpacing(3);

 // vMainlayout->addWidget(m_toggleButton);

    if(!minPrev){  // Full View

        //-- Remove Widgets from Laouts
        HlayoutDuration->removeWidget(m_slider);
        HlayoutDuration->removeWidget(m_labelDuration);
        HlayoutDuration->removeWidget(m_labelCurrentTime);

        hlayout->removeWidget(widgetButtons);

         vMainlayout->removeWidget(m_toggleButton);
        vMainlayout->removeWidget(m_WidgetImg);
        vMainlayout->removeWidget(widgetMini);
        vMinilayout->removeWidget(m_WidgetInfo);
        vMinilayout->removeItem(hlayout);
        vMinilayout->removeItem(HlayoutDuration);
        hMainlayout->removeItem(vMainlayout);

        //-- Add Widgets To Laouts
        setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        m_muteButton->setVisible(true);
        m_volumeSlider->setVisible(true);
        widgetMini->setVisible(false);
      // m_toggleButton->setVisible(false);
        m_labelCurrentTime->setVisible(false);
        hMainlayout->setMargin(6);

        HlayoutDuration->addWidget(m_muteButton);
        HlayoutDuration->addWidget(m_volumeSlider);


        vlayoutDuration->addLayout(HlayoutDuration);
        vlayoutDuration->addWidget(m_labelDuration);

         hlayout->addWidget(m_toggleButton);
        hlayout->addWidget(widgetButtons);
        hlayout->addWidget(m_WidgetInfo);
        hlayout->addLayout(vlayoutDuration);


        vMainlayout->addLayout(hlayout);
        vMainlayout->addWidget(m_slider);

        hMainlayout->addWidget(m_WidgetImg);
     //   hMainlayout->addLayout(vMainlayout);


    }else{   // Mimi View

        //-- Remove Widgets from Laouts
        HlayoutDuration->removeWidget(m_muteButton);
        HlayoutDuration->removeWidget(m_volumeSlider);
        vlayoutDuration->removeWidget(m_labelDuration);
        vlayoutDuration->removeItem(HlayoutDuration);

        hlayout->removeWidget(widgetButtons);
        hlayout->removeWidget(m_WidgetInfo);
        hlayout->removeItem(vlayoutDuration);
hlayout->removeWidget(m_toggleButton);

        vMainlayout->removeWidget(m_slider);
        vMainlayout->removeItem(hlayout);

        hMainlayout->removeWidget(m_WidgetImg);
        hMainlayout->removeItem(vMainlayout);

         //-- Add Widgets To Laouts
        setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        m_muteButton->setVisible(false);
        m_volumeSlider->setVisible(false);
        widgetMini->setVisible(true);
        m_labelCurrentTime->setVisible(true);
        m_toggleButton->setVisible(true);

        hMainlayout->setContentsMargins(20,10,20,20);


        HlayoutDuration->addWidget(m_labelCurrentTime);
        HlayoutDuration->addWidget(m_slider);
        HlayoutDuration->addWidget(m_labelDuration);


        hlayout->addWidget(widgetButtons);

        vMinilayout->addWidget(m_WidgetInfo);
        vMinilayout->addLayout(HlayoutDuration);
        vMinilayout->addLayout(hlayout);

   vMainlayout->addWidget(m_toggleButton);
        vMainlayout->addWidget(m_WidgetImg);
        vMainlayout->addWidget(widgetMini);



    }

     hMainlayout->addLayout(vMainlayout);
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

       qDebug()<<"m_duration"<<m_duration;
    //QString tStr;
    QString total,current;

    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
                        m_duration % 60, (m_duration * 1000) % 1000);

        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";

        total=totalTime.toString(format);
        //  tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
        current=currentTime.toString(format);
    }

    if(mMinPreview){
        m_labelDuration->setText(total);
        m_labelCurrentTime->setText(current);
    }else{
        m_labelDuration->setText(total+ " / " +current) ;
    }



}

void PlayerControls::setSeekSliderValueChanged(int val)
{

    if(val>m_duration)
        val=QVariant(m_duration).toInt();

    if(val<0)
        val=0;

    QMetaObject::invokeMethod(parent(), "setSeek",Q_ARG(int,val));

}


