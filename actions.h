#ifndef ACTIONS_H
#define ACTIONS_H
#include "tumb.h"
#include <QObject>
#include <QWidget>
#include <QActionGroup>
#include <QMenu>

#define P_PlayOne    0
#define P_RepeatOne  1
#define P_Squent     2
#define P_Repeat     3
#define P_Random     4

class ACtions : public QObject
{
    Q_OBJECT
public:
    explicit ACtions(QObject *parent = nullptr);
    static ACtions *instance();
    //--  playBack
    static QAction  *PlayAct()       {return instance()-> actPlay;}
    static QAction  *PauseAct()      {return  instance()->actPause;}
    static QAction  *NextAct()       {return  instance()->actNext;}
    static QAction  *PrevAct()       {return  instance()->actPrev;}
    static QAction  *PlayPauseAct()  {return  instance()->actPlayPause;}
    static QAction  *StopAct()       {return  instance()->actStop;}

    //-- Volume
    static QAction  *volumeAct()     {return  instance()->actVolume;}
    static QAction  *volumeUpAct()     {return  instance()->actVolumeUp;}
    static QAction  *volumeDownAct()     {return  instance()->actVolumeDown;}
    static QAction  *muteUnmuteAct()     {return  instance()->actMuteUnmute;}

    static void  setPlayIcon(){instance()-> actPlayPause->setIcon(instance()->playIcon);}
    static void  setPauseIcon(){instance()-> actPlayPause->setIcon(instance()->pauseIcon);}

    //-- Main
    static QAction  *openFilesAct(){return instance()->actOpenFile;}
    static QAction  *openOpenUrlsAct(){return instance()->actOpenUrl;}
    static QAction  *showSettingsAct(){return instance()->actShowSettings;}
    static QAction  *swichMimiPlayerAct(){return instance()->actSwich;}
    static QAction  *quitAct(){return instance()->actQuit;}
    static QAction  *updateAllAct(){return instance()->actUpdateAll;}
    static QAction  *searchAct(){return instance()->actSearch;}
    static QAction  *rmNonExistAct(){return instance()->actRmNonExist;}
    //-- Paylist
    static QMenu  *menuPlayBackMode(){return instance()->mnuPlayMode;}
    static QAction  *modeAct(){return instance()->actMode;}
    static QAction  *removeItemAct(){return instance()->actRemoveItem;}
    static QAction  *cleanListAct(){return instance()->actCleanList;}

    //-- help
    static QAction  *helpAct(){return instance()->actHelp;}
    static QAction  *aboutAct(){return instance()->actAbout;}
    static QAction  *aboutQtAct(){return instance()->actAboutQt;}


    //-- Style
    static QString stylShete(int h);
signals:
    //--  playBack
    void play();
    void Pause();
    void Stop();
    void previous();
    void next();
    void playPause();
   //--Volume
    void volumeUpChanged();
     void volumeDownChanged();
     void muteUnmuteChanged(bool);

    //-- Main
    void openFiles();
    void openOpenUrls();
    void showSettingsClicked();
    void swichMimiPlayer(bool);
     void updateAll();
     void rmNonExist();
    void quit();
void showSearchChanged();
void showOptions();
    //-- PLayalist
    void playbackModeChanged(int);
    void removeItem();
    void clearList();
public slots:

private:
    //--  playBack
    QAction *actPlay;
    QAction *actPause;
    QAction *actNext;
    QAction *actPrev;
    QAction *actPlayPause;
    QAction *actStop;
    //-Volume
    QAction *actVolumeUp;
    QAction *actVolumeDown;
    QAction *actMuteUnmute;
   QAction *actVolume;
    //-- Main
    QAction *actOpenFile;
    QAction *actOpenUrl;
    QAction *actShowSettings;
    QAction *actSwich;
    QAction *actQuit;
   QAction *actUpdateAll;
  QAction *actSearch;
   QAction *actRmNonExist;
   // QAction *actOptions;
    //-- PlayList Mode
    QMenu *mnuPlayMode;
    QActionGroup *actGroup;

    QAction *actPlayOne;
    QAction *actRepeatOne;
    QAction *actSquent;
    QAction *actRepeatAlbum;
    QAction *actRandom;

    QAction *actMode;

    QAction *actRemoveItem;
    QAction *actCleanList;

    QAction *actHelp;
    QAction *actAbout;
    QAction *actAboutQt;

    QIcon playIcon;
    QIcon pauseIcon;
public slots:
    void setupIcons();
private slots:
    void setPlayMode(QAction *action);
    void about();
    void help();

}
;

#endif // ACTIONS_H
