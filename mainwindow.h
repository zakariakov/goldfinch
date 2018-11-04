#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mycontentmodel.h"
#include "mylistmodel.h"
#include "treeviewcontent.h"
#include "listview.h"
#include "treeviewaudio.h"
#include "mediaupdate.h"
#include "database.h"
#include "listitemdelegate.h"
#include "treeitemdelegate.h"
#include "player.h"
#include "playercontrols.h"
#include "setting.h"
#include "widgetplaylist.h"
#include "widgetimageinfo.h"
#include "searchbar.h"
#include <QSystemTrayIcon>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     Player *player(){return mPlayer;}
signals:
    void iconsChanged();

private:
    Ui::MainWindow *ui;
    //!
    void changeEvent(QEvent *event);
    //!
    void changeStyleSheet();

    void creatTrayIcon();
    //!
    QMap<QString,QString>mMap;
    //! موديول قائمة النفاصبل
    MyContentModel *mMyTreeModel;
    //! موديول القائمة الايقونات
    MyListModel *mMyListModel;
    //! شجرة عرض التفاصبل
    TreeViewContent *mTVContent;
    //!
    TreeViewAudio *mTVAudio;
    //!
    ListView *mListView;
    //!  حاوي القائمة وادات تجرير القائمة
    WidgetPlayList *mWPlayList;
    //! قاغدة البيانات
 ///   DataBase *mDataBase;
    //! تحرير مظهر الايقونات في معرض القائمة
    ListItemDelegate *mLIDelegate;
    //!
    TreeItemDelegate *mTItemDelegate;
    //! وحدة القراءة
    Player *mPlayer;
    //! التحكم في ازرار القراءة
    PlayerControls *controls;
    //!
    WidgetImageInfo *mImageInfo;
    //! تحديث المقطوعات من الجهاز
    MediaUpdate *mMediaUpdate;

    SearchBar *mSearchBar;
    //!
    Setting *mSetting;
    //!
    QAction *mActSwich;
    //!
    QSlider *mSliderIconSize;
    //!
    int mIconSize=94;
    //!
    QString mcoloor;
    //!
    QSystemTrayIcon *trayIcon=nullptr;


private slots:
    //!
    void showRaise(){showNormal();raise();}
    //!
    void switchViewMode(bool mini);
    //!
    void chargeRecent();
    //! تحميل الايقونات
    void setupIcons();
    //! اعادة التحميل بعد اكتمال التحديث
    void rechargeAlbums();
    //! عرض نسبة التقدم
    void progressShow(int max);
    //! نسبة التقدم
    void progressSetValue(int value);
    //! عرض مسار الالبومات
    void changeStatusPathText();
    //! تغيير مسار الالبومات
    void setAlbumPath(int index);
     //!
    void setIconSize(int value);
    //!
    void setwTitle(const QString &title);
    //!
    void chargeListItemes();
    //!
    void searchAudios(int col,const QString &text);
    //!
    void setlabelImage();
    //!
    void onTreeViewContentActivated(const QModelIndex &index);
    //!
    void onListViewActivated(const QModelIndex &index);
    //!
    void on_tb_paneContent_toggled(bool checked);
    //!
    void on_tb_panePlaylist_toggled(bool checked);
    //!
    void on_tb_addAlbum_clicked();
    //!
    void on_tb_playAlbum_clicked();
    //!
    void onChangeImageAlbum(int row);
//!
   void changeImageAlbum(int id ,const QString Name);
//!
      void on_tb_imgAlbum_clicked();

    //!
    void onFavoritAlbum(int row, bool favo);
    //!
    void onAddAlbum(int row);
    //!
    void onPlayAlbum(int row);
    //!
    void on_tb_favoritAlbum_clicked();
    //!
    void onTreeAudioClicked(const QModelIndex &index);
    //!
    void onTreeAudioActivated(const QModelIndex &index);
    //!
   // void editCurIndex(const QString &path);
    //!
    void onActionopentriggered();


    void on_tButtonCancelMsg_clicked();
    void on_tButtonOkMsg_clicked();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif // MAINWINDOW_H
