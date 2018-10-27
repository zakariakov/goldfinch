#ifndef WIDGETPLAYLIST_H
#define WIDGETPLAYLIST_H
//#include "playlistview.h"
#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QListView>
#include <QMenu>

class WidgetPlayList : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetPlayList(QWidget *parent = nullptr);

    QListView *playListView(){return mPlayListView;}
signals:
    void rmovePlaylistItem(QModelIndex);
    void movCurentItem(int,int);
    void cleanList();
    void playbackModeChanged(int);
public slots:
    void setupIcons();
private slots:
    void rmoveItem();
    void moveItemUp();
    void moveItemDown();
    void setPlayMode();
private:
    QToolButton *tbRemoveItem;
    QToolButton *tbMoveItemUp;
    QToolButton *tbMoveItemDown;
    QToolButton *tbCleanList;
    QToolButton *tbPlayMode;

    QListView *mPlayListView;

//    QIcon cleanIcon;
//    QIcon removeIcon;
//    QIcon playModeIcon;
//    QIcon playoneIcon;
//    QIcon repeatoneIcon;
//    QIcon squentialIcon;
//    QIcon repaeatAlbumIcon;
//    QIcon shufleAlbumIcon;

    QAction *actPlayOne;
    QAction *actRepeatOne;
    QAction *actSquent;
    QAction *actRepeatAlbum;
    QAction *actRandom;

};

#endif // WIDGETPLAYLIST_H
