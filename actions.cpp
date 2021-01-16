#include "actions.h"

#include  <QAction>
#include  <QApplication>
#include  <QMessageBox>
#include  <QDebug>
Q_GLOBAL_STATIC(ACtions, ACtionsInstance)
ACtions *ACtions::instance()
{
    return ACtionsInstance();
}

ACtions::ACtions(QObject *parent) : QObject(parent)
{

    //-- PlayBack
    actPlay=new QAction(tr("Play"));
    actPlay->setShortcut(Qt::Key_MediaPlay);
    actPlay->setToolTip(tr("Play \n")+actPlay->shortcut().toString());
    connect(actPlay,&QAction::triggered,this,&ACtions::play);

    actPlayPause=new QAction(tr("Play/Pause"));
    actPlayPause->setShortcut(Qt::Key_Space);
    actPlayPause->setToolTip(tr("Play/Pause \n")+actPlayPause->shortcut().toString());
    connect(actPlayPause,&QAction::triggered,this,&ACtions::playPause);

    actPause=new QAction(tr("Pause"));
    actPause->setShortcut(Qt::Key_MediaPause);
    actPause->setToolTip(tr("Pause \n")+actPause->shortcut().toString());
    connect(actPause,&QAction::triggered,this,&ACtions::Pause);

    actStop=new QAction(tr("Stop"));
    actStop->setShortcut(Qt::Key_MediaStop);
    actStop->setToolTip(tr("Stop \n")+actStop->shortcut().toString());
    connect(actStop,&QAction::triggered,this,&ACtions::Stop);

    actNext=new QAction(tr("Next"));
    actNext->setShortcut(QKeySequence("Ctrl+Shift+Left"));
    actNext->setToolTip(tr("Next Song \n")+actNext->shortcut().toString());
    connect(actNext,&QAction::triggered,this,&ACtions::next);

    actPrev=new QAction(tr("previous"));
    actPrev->setShortcut(QKeySequence("Ctrl+Shift+Right"));
    actPrev->setToolTip(tr("Previous Song \n")+actPrev->shortcut().toString());
    connect(actPrev,&QAction::triggered,this,&ACtions::previous);

    //--Seek
    actSeekPlus=new QAction(tr("seek +10"));
    actSeekPlus->setShortcut(QKeySequence("Ctrl+Shift+Up"));
    actSeekPlus->setToolTip(tr("seek +10 second \n")+actSeekPlus->shortcut().toString());
    connect(actSeekPlus,&QAction::triggered,this,&ACtions::seekPlusChanged);

    actSeekMinus=new QAction(tr("seek -10"));
    actSeekMinus->setShortcut(QKeySequence("Ctrl+Shift+down"));
    actSeekMinus->setToolTip(tr("seek -10 second \n")+actSeekMinus->shortcut().toString());
    connect(actSeekMinus,&QAction::triggered,this,&ACtions::seekMinusChanged);



actToggleLibrery=new QAction(tr("Show/Hide Librery"));
actToggleLibrery->setShortcut(QKeySequence("Ctrl+T"));
actToggleLibrery->setToolTip(tr("Show/Hide Librery \n")+actToggleLibrery->shortcut().toString());
connect(actToggleLibrery,&QAction::triggered,this,&ACtions::toggleLibreryChanged);
connect(actToggleLibrery,&QAction::triggered,this,&ACtions::toggleLibreryIcon);

actToggleLibrery->setCheckable(true);

//-Volume
    actVolumeUp     =new QAction(tr("Increase volume"));
    actVolumeUp->setShortcut(QKeySequence("Ctrl+Shift++"));
    connect(actVolumeUp,&QAction::triggered,this,&ACtions::volumeUpChanged);

    actVolumeDown=new QAction(tr("Decrease volume"));
    actVolumeDown->setShortcut(QKeySequence("Ctrl+Shift+-"));
    connect(actVolumeDown,&QAction::triggered,this,&ACtions::volumeDownChanged);

    actMuteUnmute=new QAction(tr("Mute/Unmute"));
    actMuteUnmute->setShortcut(QKeySequence("Ctrl+Shift+N"));
    connect(actMuteUnmute,&QAction::triggered,this,&ACtions::muteUnmuteChanged);
    actMuteUnmute->setCheckable(true);

    actVolume=new QAction(tr("Volume"));
    actVolume->setCheckable(true);


    //-- Main
    actOpenFile=new QAction(tr("Open Files"));
    actOpenFile->setShortcut(QKeySequence("Ctrl+O"));
    connect(actOpenFile,&QAction::triggered,this,&ACtions::openFiles);

    actOpenUrl=new QAction(tr("Open Urls"));
    connect(actOpenUrl,&QAction::triggered,this,&ACtions::openOpenUrls);

    actShowSettings=new QAction(Tumb::icon(I_PROPERTIES),tr("Settings"));
    actShowSettings->setShortcut(QKeySequence("Ctrl+Shift+O"));
    connect(actShowSettings,&QAction::triggered,this,&ACtions::showSettingsClicked);

    actSwich=new QAction(tr("Mini player"));
    actSwich->setShortcut(QKeySequence("Ctrl+M"));
    actSwich->setCheckable(true);
    actSwich->setToolTip(tr("Mini player \n")+actSwich->shortcut().toString());
    connect(actSwich,&QAction::triggered,this,&ACtions::swichMimiPlayer);



            actShowMenu=new QAction(tr("show/hide menu"));
            actShowMenu->setShortcut(QKeySequence("Ctrl+Shift+M"));
            actShowMenu->setCheckable(true);
            actShowMenu->setToolTip(tr("show/hide menu \n")+actShowMenu->shortcut().toString());
            connect(actShowMenu,&QAction::triggered,this,&ACtions::showMenuChanged);


    actQuit=new QAction(tr("Quit"));
    actQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actQuit,&QAction::triggered,this,&ACtions::quit);

    actUpdateAll=new QAction(tr("Update All"));
    actUpdateAll->setShortcut(QKeySequence("Ctrl+U"));
    connect(actUpdateAll,&QAction::triggered,this,&ACtions::updateAll);

    actRmNonExist=new QAction(tr("Rmove Non Existing files"));
    actRmNonExist->setShortcut(QKeySequence("Ctrl+R"));
    connect(actRmNonExist,&QAction::triggered,this,&ACtions::rmNonExist);

    actSearch=new QAction(Tumb::icon(I_FIND),tr("Search"),this);
    actSearch->setShortcut(QKeySequence("Ctrl+F"));
    connect(actSearch,&QAction::triggered,this,&ACtions::showSearchChanged);

    // -- playlist Mode
    mnuPlayMode=new QMenu(tr("PlayBack Mode"));

    actPlayOne=mnuPlayMode->addAction(tr("Current Once"));
    actPlayOne->setData(P_PlayOne);
    actPlayOne->setCheckable(true);

    actRepeatOne=mnuPlayMode->addAction(tr("Current Loop"));
    actRepeatOne->setData(P_RepeatOne);
    actRepeatOne->setCheckable(true);

    actSquent=mnuPlayMode->addAction(tr("Sequential"));
    actSquent->setData(P_Squent);
    actSquent->setCheckable(true);

    actRepeatAlbum=mnuPlayMode->addAction(tr("Plylist Loop"));
    actRepeatAlbum->setData(P_Repeat);
    actRepeatAlbum->setCheckable(true);

    actRandom=mnuPlayMode->addAction(tr("Random"));
    actRandom->setData(P_Random);
    actRandom->setCheckable(true);

    actRemoveItem=new QAction(tr("Remove"));
    actRemoveItem->setToolTip(tr("Remove Current "));
    connect(actRemoveItem,&QAction::triggered,this,&ACtions::removeItem);

    actCleanList=new QAction(tr("Clear PlayList"));
    actCleanList->setToolTip(tr("Clear PlayList"));
    connect(actCleanList,&QAction::triggered,this,&ACtions::clearList);


    actMode=new QAction;

    actGroup=new QActionGroup(this);
    actGroup->addAction(actPlayOne);
    actGroup->addAction(actRepeatOne);
    actGroup->addAction(actSquent);
    actGroup->addAction(actRepeatAlbum);
    actGroup->addAction(actRandom);

    connect(actGroup,&QActionGroup::triggered,this,&ACtions::setPlayMode);

    actSquent->setChecked(true);

    //--help
    actHelp=new QAction(tr("Help"));
    connect(actHelp,&QAction::triggered,this,&ACtions::help);
    actAbout=new QAction(tr("About"));
    connect(actAbout,&QAction::triggered,this,&ACtions::about);
    actAboutQt=new QAction(tr("About Qt"));
    connect(actAboutQt,&QAction::triggered,qApp,&QApplication::aboutQt);


    setupIcons();

}


//----------------------------------------
void ACtions::setupIcons()
{

    playIcon=Tumb::icon(I_PLAY);
    pauseIcon=Tumb::icon(I_PAUSE);

    actPause->setIcon(pauseIcon);
    actPlay->setIcon(playIcon);
    actPlayPause->setIcon(playIcon);
    actNext->setIcon(Tumb::icon(I_NEXT));
    actPrev->setIcon(Tumb::icon(I_PREV));
    actVolume->setIcon(Tumb::icon(I_VOLUME));
    actStop->setIcon(Tumb::icon(I_STOP));
actToggleLibrery->setIcon(Tumb::icon(I_PANE_SHOW));
    actPlayOne->setIcon(Tumb::icon(I_M_PLAY_ONE));
    actRepeatOne->setIcon(Tumb::icon(I_M_REPEAT_ONE));
    actSquent->setIcon(Tumb::icon(I_M_SEQUEN));
    actRepeatAlbum->setIcon(Tumb::icon(I_M_REPEAT));
    actRandom->setIcon(Tumb::icon(I_M_SHUFLE));

    actMode->setIcon(Tumb::icon(I_M_SEQUEN));


    actRemoveItem->setIcon(Tumb::icon(I_LIST_REMOVE));
    actCleanList->setIcon(Tumb::icon(I_CLEAN));

       actShowSettings->setIcon(Tumb::icon(I_PROPERTIES));
       actSwich->setIcon(Tumb::icon(I_TOGGLE));
}

void ACtions::help()
{


}

void ACtions::about()
{
    QString txt="Copyright (C) 2014 AbouZakaria <zakariakov@gmail.com>.\n";
            txt+="This program is free software.\n";
             txt+="GNU General Public License 3 \n";
             txt+="Source: https://github.com/zakariakov/goldfinch";
   QMessageBox::information(nullptr,tr("About...Goldfinch"),txt);
}


void ACtions::setPlayMode(QAction *action)
{
    if(action)
    {
        int data=action->data().toInt();
     //   qDebug()<<"playmode="<<data;
        emit playbackModeChanged(data);
     actMode->setIcon(action->icon());

    }
}
void ACtions::toggleLibreryIcon(bool checked)
{
    if(checked)
        actToggleLibrery->setIcon(Tumb::icon(I_PANE_HIDE));
    else
        actToggleLibrery->setIcon(Tumb::icon(I_PANE_SHOW));

}

QString ACtions::stylShete(int h/*,int tb_h*/)
{
int w_radius=h/2;
int b_radius=w_radius-2;
QColor h_color=QGuiApplication::palette().highlight().color();
QString red=QString::number(h_color.red());
QString green=QString::number(h_color.green());
QString blue=QString::number(h_color.blue());

QString h_rgba=QString("rgba(%1,%2,%3,150)").arg(red).arg(green).arg(blue) ;

QString shadow=QString("rgba(%1,%2,%3,100)").arg(red).arg(green).arg(blue) ;



     QString style=QString(

                " QWidget#widgetCurAlbum{"
                   "  background-color:palette(Window);"
                   "  color: palette(window-text);"
                " }"

               " QWidget#widgetView{"
               "  border:1px solid palette(highlight);"
               "  border-radius:2px;"
               " }"

                /*-- search bar --*/
                "QWidget#SearchBar{"
                "   background-color: palette(base);"
                "   border:1px solid palette(highlight);"
                "   border-radius:%1px;"
                " min-height:%3px;"
                " max-height:%3px;"

                "}"
                "QToolButton#TBSearch{"
                "   background-color: palette(base);"
                "   border:0px ;"
                "   border-radius: %2px;"
                 " min-height: %4px;"
                 " min-width: %4px;"
                "}"
                "QToolButton#TBSearch:hover{ background-color: palette(shadow);}"
                "QToolButton#TBSearch:pressed{background-color: %5;}"
                /*-- TreeView ListView --*/
                "QTreeView{"
                "background-color: palette(Window);"
                      "color:palette(window-text);"
                     "background-attachment: scroll;"
                "}"
                " QListView{"
                    "background-color: palette(Window);"
                    "color:palette(window-text);"
                " }"

                /*-- Player Control --*/
                "QToolButton#PrevButton ,#NextButton ,#VolumeButton{"
                "border-radius: 15px;"
                "width: 30px;"
                "height:30px;"
                " max-width: 30px;"
                " max-height:30px;"
                "background-color:transparent"
                "}"

                "QToolButton#PlayButton{"
                "border-radius: 15px;"
                "width: 30px;"
                "height:30px;"
                " max-width: 30px;"
                " max-height:30px;"
                "background-color:%5"
                "}"

                "QToolButton#PrevButton:hover,#PlayButton:hover,#NextButton:hover{"
                "background-color: palette(Highlight);"
                "}"

                "QToolButton#PrevButton:pressed,#PlayButton:pressed,#NextButton:pressed{"
                "background-color:palette(shadow)"
                "}"


                "QToolButton#VolumeButton:hover ,#VolumeButton:pressed{"
                                "background-color:%5"
                "}"

                "QToolButton#ToggleButton{"
                "border-radius: 5px;"
                "width: 20px;"
                "height:20px;"
                " max-width: 20px;"
                " max-height:20px;"
                "background-color:transparent"
                "}"

                "QToolButton#ToggleButton:hover{"
                   "background-color:%5"
                "}"

/*ToggleButton*/
                /*-- VolumeSlider --*/
                " QSlider#VolumeSlider::groove:horizontal {"
                " height: 8px;"
                " background: transparent;"
                " margin: 6px 0px;"
                " }"

                " QSlider#VolumeSlider::handle:horizontal {"
                " background: palette(Highlight);"
                " width: 14px;"
                " margin: -3px 0px; "
                " border-radius: 7px;"
                "}"
                ).arg(w_radius).arg(b_radius).arg(h).arg(h-4).arg(shadow);

    if(QApplication::isLeftToRight()){
       style+=QString(
                " QSlider#VolumeSlider::add-page:horizontal {"
                " background:%1 ;"
                " margin: 8px 0; "
                " border-radius: 2px;"
                " }"

                " QSlider#VolumeSlider::sub-page:horizontal {"
                " background: palette(Highlight);"
                " margin: 8px 0; "
                " border-radius: 2px;"
                " }").arg(shadow);
    }else{
       style+=QString(
                " QSlider#VolumeSlider::add-page:horizontal {"
                " background:palette(Highlight ) ;"
                " margin: 8px 0; "
                " border-radius: 2px;"
                " }"

                " QSlider#VolumeSlider::sub-page:horizontal {"
                " background:%1;"
                " margin: 8px 0; "
                " border-radius: 2px;"
                " }").arg(shadow);
    }
    return style;

}
