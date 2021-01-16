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
//#include "widgetimageinfo.h"
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
    void setUrl(const QString &file);
signals:
   // void iconsChanged();

private:
    Ui::MainWindow *ui;
    //! مراقبة تغير السمة العامة للنظام
    void changeEvent(QEvent *event);
    //! تغيرر السمة في حال تغيرت سمة النظام
    void changeStyleSheet();
    //! تكوين ايقونة صينية النظام
    void creatTrayIcon();
    //! المعلومات العامة عن النوان الحالي للعرض
  QVariantMap mMap; //TODO Change to qvariant
    //! موديول قائمة النفاصبل
    MyContentModel *mMyTreeModel;
    //! موديول القائمة الايقونات
    MyListModel *mMyListModel;
    //! شجرة عرض التفاصبل
    TreeViewContent *mTVContent;
    //! شجرة عرض الملفات الصوتية
    TreeViewAudio *mTVAudio;
    //! قائمة عرض ايقونات الالبومات والفنانين
    ListView *mListView;
    //!  حاوي القائمة وادات تجرير القائمة
    WidgetPlayList *mWPlayList;
    //! تحرير مظهر الايقونات في معرض القائمة
    ListItemDelegate *mLIDelegate;
    //!تحرير مظهر الايقونات في معرض الشجرة
    TreeItemDelegate *mTItemDelegate;
    //! وحدة القراءة
    Player *mPlayer;
    //! واجهة عرض صورة الالبوم والمعلومات
  //  WidgetImageInfo *mImageInfo;
    //! تحديث المقطوعات من الجهاز
    MediaUpdate *mMediaUpdate;
    //! وحدة التحكم في الاضافة والتحديث
    SearchBar *mSearchBar;
    //! لوحة البحث
    Setting *mSetting;
    //!  امر تغيير الرض المصغر
   // QAction *mActSwich;
    //!  المتحكم في حجم الايقونات
    QSlider *mSliderIconSize;
    //! حجم الايقونات
    int mIconSize=94;
    //! لون العام للبرناج يتم مقارنته في حال تغير السمة
    QString mcoloor;
       QString mTitle;
    //! ايقونة صينية النظام
    QSystemTrayIcon *mTrayIcon=nullptr;

    //!
    bool mShowNotification=false;
QToolButton *tButtonMenu;
private slots:
    //! جلب النافذة الى الاعلى
    void showRaise(){showNormal();raise();}
    //! تغيير العرض بين مصغر وكامل
    void switchViewMode(bool mini);
    //!
     void showHideMenu(bool show);
    //! تحميل الاعدادات السابقة
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
    //! تغيير حجم الايونات
    void setIconSize(int value);
    //! تغيير عنوان النافذة واخطار في حالة التصغير او الاخفاء
    void setwTitle(const QString &title, const QString &info);
    //! تحميل قائمة الالبومات او الفنانين او الملفات
    void chargeListItemes();
    //! البحث عن ملفات
    void searchAudios(int col,const QString &text);
    //! تغيير عموان القائمة
    void setlabelImage();
    //! حذف الملفات الغير موجودة
    void removeNonExisting();
    //! السحب ةالافلات
    void dragEnterEvent(QDragEnterEvent *e);
    //! السحب ةالافلات
    void dropEvent(QDropEvent *e);
    //!
    void onTreeViewContentActivated(const QModelIndex &index);
    //!
    void onListViewActivated(const QModelIndex &index);
    //!
//    void on_tb_paneContent_toggled(bool checked);
//    //!
//    void on_tb_panePlaylist_toggled(bool checked);
    //!
    void playlist_toggled(bool checked);
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
    void onActionopentriggered();
     //!
    void on_tButtonCancelMsg_clicked();
     //!
    void on_tButtonOkMsg_clicked();
     //!
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void showPropertyDialog(bool isRead, const QString &file);

    void toggleHide(){isHidden() ? show() : hide();}

    void onQuit();

    void saveSettings();

    void showSettings();

};

#endif // MAINWINDOW_H
