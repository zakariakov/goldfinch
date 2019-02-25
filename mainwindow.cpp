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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actions.h"
#include "dialogoptions.h"
#include "propertiesfile.h"
#include <QMediaMetaData>
#include <QFileInfo>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFontMetrics>
#include <QHeaderView>
#include <QProcess>
#include <dialogopen.h>
#include <QFileDialog>
#include <QStandardPaths>
#include <QComboBox>
#include <QStyleFactory>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"CACHE"<<CACHE;

    mMyTreeModel=   new MyContentModel;
    mMyListModel=   new MyListModel;
    mMediaUpdate=   new MediaUpdate;
    mTVContent=     new TreeViewContent;
    mListView=      new ListView;
    mTVAudio=       new TreeViewAudio;
    mLIDelegate=    new ListItemDelegate;
    mTItemDelegate= new TreeItemDelegate;
    mImageInfo=     new WidgetImageInfo;
    mSearchBar=     new SearchBar(this);
    mWPlayList=     new WidgetPlayList;
    mPlayer=        new Player(mWPlayList->playListView(),this);

    //--menuFile
    ui->menuFile->addAction(ACtions::openFilesAct());
     ui->menuFile->addSeparator();
    ui->menuFile->addAction(ACtions::showSettingsAct());
     ui->menuFile->addSeparator();
     ui->menuFile->addAction(ACtions::searchAct());
    ui->menuFile->addAction(ACtions::swichMimiPlayerAct());
     ui->menuFile->addSeparator();
    ui->menuFile->addAction(ACtions::updateAllAct());
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ACtions::quitAct());

    //--menuEdit
     ui->menuEdit->addAction(ACtions::PlayPauseAct());
    ui->menuEdit->addAction(ACtions::PlayAct());
    ui->menuEdit->addAction(ACtions::PauseAct());
    ui->menuEdit->addAction(ACtions::PrevAct());
    ui->menuEdit->addAction(ACtions::NextAct());
    ui->menuEdit->addAction(ACtions::StopAct());
    ui->menuEdit->addSeparator();
      ui->menuEdit->addAction(ACtions::volumeUpAct());
      ui->menuEdit->addAction(ACtions::volumeDownAct());
      ui->menuEdit->addAction(ACtions::muteUnmuteAct());
         ui->menuEdit->addSeparator();
    ui->menuEdit->addMenu(ACtions::menuPlayBackMode());

    //-- menuHelp
    ui->menuHelp->addAction(ACtions::helpAct());
    ui->menuHelp->addAction(ACtions::aboutAct());
    ui->menuHelp->addAction(ACtions::aboutQtAct());

    //-- Slider Change Icons Size
    mSliderIconSize = new QSlider(Qt::Horizontal, this);
    mSliderIconSize->setRange(96,256);
    mSliderIconSize->setMaximumWidth(160);
    ui->statusBar->addPermanentWidget(mSliderIconSize);

    //-- ACtion Search
    QAction *actSearch=new QAction(Tumb::icon(I_FIND),tr("Search"),this);
    actSearch->setShortcut(QKeySequence("Ctrl+F"));
    // actSearch->setCheckable(true);
    connect(actSearch,&QAction::triggered,mSearchBar,&SearchBar::showHid);

    //--  Menu Mainubutton;
    tButtonMenu=new QToolButton;
    tButtonMenu->setObjectName("tButtonMenu");
   tButtonMenu->setPopupMode(QToolButton::InstantPopup);
   tButtonMenu->setMenu(ui->menuFile);

    //--  TreeViewContent;
    mTVContent->setModel(mMyTreeModel);
    mTVContent->setHeaderHidden(true);
    ui->vLayoutContent->addWidget(mTVContent);

    //--  ListView
    mListView->setItemDelegate(mLIDelegate);
    ui->vLayoutView->addWidget(mListView);
    mListView->setModel(mMyListModel);

    //--  TreeViewAudio
    mTVAudio->setItemDelegate(mTItemDelegate);

    mTVAudio->setModel(mMyListModel);

    ui->hlLayoutAudios->addWidget(mTVAudio);

    ui->splitter->addWidget(mWPlayList);

// ui->hLayoutControle->insertWidget(0,mPlayer);
  ui->toolBar->addWidget(mPlayer);
  ui->toolBar->addWidget(tButtonMenu);
        ui->vLayoutViewAll->insertWidget(0,mSearchBar);

    ui->progressBarUpdate->setVisible(false);

    ui->widgetMessage->setVisible(false);

    mSearchBar->setVisible(false);



    //   ------------------------        CONNECTIONS      ------------------------

    connect(ACtions::instance(), &ACtions::showSettingsClicked, this,&MainWindow::showSettings);
    connect(ACtions::instance(), &ACtions::swichMimiPlayer, this,&MainWindow::switchViewMode);
    connect(ACtions::instance(), &ACtions::quit, this,&MainWindow::onQuit);
    connect(ACtions::instance(), &ACtions::openFiles,this,&MainWindow::onActionopentriggered);
    connect(ACtions::instance(), &ACtions::updateAll,mMediaUpdate,&MediaUpdate::updateAllDirectories);
    connect(ACtions::instance(), &ACtions::volumeDownChanged,mPlayer,&Player::setVolumeDown);
    connect(ACtions::instance(), &ACtions::volumeUpChanged,mPlayer,&Player::setVolumeUp);
    connect(ACtions::instance(), &ACtions::muteUnmuteChanged,mPlayer,&Player::setVolumeMuteUnmute);
    connect(ACtions::instance(), &ACtions::showSearchChanged,mSearchBar,&SearchBar::showHid);

    //--------------------------------------------------------------------------------

    connect(mWPlayList,  &WidgetPlayList::rmovePlaylistItem,  mPlayer,    &Player::rmovePlaylistItem);
    connect(mWPlayList,  &WidgetPlayList::movCurentItem,      mPlayer,    &Player::moveMedia);
    connect(mWPlayList,  &WidgetPlayList::cleanList,          mPlayer,    &Player::cleanList);
    connect(mWPlayList,  &WidgetPlayList::playbackModeChanged,mPlayer,    &Player::setPlaybackMode);
    connect(mWPlayList,  &WidgetPlayList::getPropperty,       this,       &MainWindow::showPropertyDialog);

    connect(mPlayer,     &Player::imageChanged,               mImageInfo,  &WidgetImageInfo::setImage);
    connect(mPlayer,     &Player::titleChanged,               mImageInfo,  &WidgetImageInfo::setTitle);
    connect(mPlayer,     &Player::appCloseChanged,            this,  &MainWindow::onQuit);
    connect(mPlayer,     &Player::appRaiseChanged,            this,&MainWindow::showRaise);
    connect(mPlayer,     &Player::appHideChanged,             this,&MainWindow::hide);
    connect(mPlayer,     &Player::apptoggleHideChanged,       this,&MainWindow::toggleHide);

    connect(mPlayer,     &Player::titleChanged,               this      ,  &MainWindow::setwTitle);

    connect(mMediaUpdate,&MediaUpdate::updated,               this, &MainWindow::rechargeAlbums);
    connect(mMediaUpdate,&MediaUpdate::progressMaxChanged,    this, &MainWindow::progressShow);
    connect(mMediaUpdate,&MediaUpdate::progressValueChanged,  this, &MainWindow::progressSetValue);
    connect(mMediaUpdate,&MediaUpdate::directoryNeedUpdate,          this->ui->widgetMessage, &QWidget::setVisible);

    connect(mLIDelegate, &ListItemDelegate::addAlbum,         this, &MainWindow::onAddAlbum);
    connect(mLIDelegate, &ListItemDelegate::playAlbum,        this, &MainWindow::onPlayAlbum);
    connect(mLIDelegate, &ListItemDelegate::favoritAlbum,     this, &MainWindow::onFavoritAlbum);
    connect(mLIDelegate, &ListItemDelegate::changeImageAlbum, this, &MainWindow::onChangeImageAlbum);

    connect(mTVContent,  &TreeViewContent::activated,         this, &MainWindow::onTreeViewContentActivated);
    connect(mTVContent,  &TreeViewContent::clicked,           this, &MainWindow::onTreeViewContentActivated);

    connect(mTVAudio,    &TreeViewAudio::clicked,             this, &MainWindow::onTreeAudioClicked);
    connect(mTVAudio,    &TreeViewAudio::treeAudioClicked,    this, &MainWindow::onTreeAudioClicked);
    connect(mTVAudio,    &TreeViewAudio::updateCurent,        this, &MainWindow::chargeListItemes);
    connect(mTVAudio,    &TreeViewAudio::activated,           this, &MainWindow::onTreeAudioActivated);
    connect(mTVAudio,    &TreeViewAudio::getProperty,         this,    &MainWindow::showPropertyDialog);

    connect(mListView,        &ListView::activated,           this,&MainWindow::onListViewActivated);
    connect(mSliderIconSize,  &QSlider::valueChanged,          this,&MainWindow::setIconSize);

    connect(mPlayer,&Player::playBackChanged,this->ui->statusBar,&QStatusBar::showMessage);
    connect(ui->comboBoxSwichCat,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::setAlbumPath);

    connect(mSearchBar,  &SearchBar::searchTextChanged,          this,&MainWindow::searchAudios);
    connect(mSearchBar,  &SearchBar::clearSearching,          mMyListModel,&MyListModel::clear);


    DataBase::openDataBase();

    //switchViewMode(false);

    changeStyleSheet();

    setupIcons();

    chargeRecent();


}

void MainWindow::switchViewMode(bool mini)
{
    QSettings settings;
    settings.beginGroup("Window");

    if(mini){
        settings.setValue("Geometry",this->saveGeometry());

        ui->widgetLibrery->setVisible(false);
        ui->statusBar->setVisible(false);
        ui->vLayout_Center->addWidget(mImageInfo);
        mImageInfo->setHorizontal(false);
        mImageInfo->setMinimumSize(QSize(0,0));
        showNormal();
        resize(360,360);
        adjustSize();
        resize(360,360);
        //
    }else{
        restoreGeometry(settings.value("Geometry").toByteArray());

        ui->widgetLibrery->setVisible(true);
        ui->statusBar->setVisible(true);
        //  ui->hLayoutControle->insertWidget(0,mImageInfo);
        //     ui->vLayoutContent->addWidget(mImageInfo);
        //     ui->toolBar->addWidget(mImageInfo);
        mWPlayList->addWidget(mImageInfo);
        mImageInfo->setMinimumSize(QSize(50,50));
        mImageInfo->setHorizontal(true);

    }
    ACtions::swichMimiPlayerAct()->setChecked(mini);
    settings.endGroup();

}

void MainWindow::setIconSize(int value)
{
    mIconSize=value;
    //  qDebug()<<value;
    Tumb::setSize(value);
    mListView->setIconSize(QSize(value,value));
    ui->widgetCurAlbum->setMinimumWidth(value+2);
    ui->widgetCurAlbum->setMaximumWidth(value+2);
    ui->labelAlbumImage->setMinimumSize(value,value);
    ui->labelAlbumImage->setMaximumSize(value,value);
    ui->widgetCurAlbum->adjustSize();
    ui->labelAlbumImage->resize(value,value);
    ui->labelAlbumImage->adjustSize();
    if(ui->stackedWidgetView->currentWidget()==ui->pageAudio)
        setlabelImage();
    // ui->labelAlbumImage->setMinimumSize(value,value);
}

MainWindow::~MainWindow()
{
   saveSettings();
    mPlayer->saveList();
    delete ui;
}

void MainWindow::onQuit()
{
   saveSettings();
   mPlayer->saveList();
   close();
    qApp->quit();
}

void MainWindow::saveSettings()
{

    Setting::saveRecent(mMap, ui->comboBoxSwichCat->currentIndex());

    QSettings settings;
    settings.beginGroup("Window");

    if(!ACtions::swichMimiPlayerAct()->isChecked()){
        settings.setValue("Geometry",this->saveGeometry());
        settings.setValue("SState",ui->splitter->saveState());
    }
    settings.setValue("MiniPlayer",ACtions::swichMimiPlayerAct()->isChecked());
    settings.setValue("IconSize",mIconSize);

    settings.endGroup();
}
// ------------------------------------------------------
void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::StyleChange || event->type()==QEvent::PaletteChange)
    {
        changeStyleSheet();
    }

}

// ------------------------------------------------------
void MainWindow::changeStyleSheet()
{

    QColor  col=this->palette().text().color();
    QColor  coled=this->palette().highlight().color();
    QString colControl=this->palette().window().color().dark().name();

    if (mcoloor==col.name()+coled.name())
        return;

    mcoloor=col.name()+coled.name();

    int h=mSearchBar->height();
    setStyleSheet(ACtions::stylShete(h/*,tb_h*/)) ;

    setupIcons();
    ACtions::instance()->setupIcons();

}

// ------------------------------------------------------
void MainWindow::chargeRecent()
{

    mMap=Setting::getRecentMap();
      int index  =Setting::getRecentndex();

      QSettings settings;
    settings.beginGroup("Window");
     ui->menuBar->setVisible(settings.value("ShowMenu",false).toBool());
    restoreGeometry(settings.value("Geometry").toByteArray());
    ui->splitter->restoreState(settings.value("SState").toByteArray());
    mIconSize=settings.value("IconSize",128).toInt();
    mSliderIconSize->setValue(mIconSize);
    bool trayicon=settings.value("TrayIcon",true).toBool();
    bool mini=settings.value("MiniPlayer",false).toBool();
     mShowNotification =settings.value("ShowNotification",false).toBool();
   QString style=    settings.value("Style",tr("Default")).toString();
   if(style!=tr("Default"))
      QApplication::setStyle( QStyleFactory::create(style));
    settings.endGroup();

    ui->comboBoxSwichCat->setCurrentIndex(index);
    setIconSize(mIconSize);
    setAlbumPath(index);
    chargeListItemes();
    if(trayicon &&  QSystemTrayIcon::isSystemTrayAvailable())
        creatTrayIcon();


    switchViewMode(mini);
}

// ------------------------------------------------------
void MainWindow::setupIcons()
{

    ui->tb_playAlbum->   setIcon(Tumb::icon(I_PLAY));
    ui->tb_favoritAlbum->setIcon(Tumb::icon(I_START));
    ui->tb_imgAlbum->    setIcon(Tumb::icon(I_IMG));
    ui->tb_addAlbum->    setIcon(Tumb::icon(I_ADD_ALBUM));
    ui->tb_paneContent-> setIcon(Tumb::icon(I_PANE_SHOW));
    ui->tb_panePlaylist->setIcon(Tumb::icon(I_PANE_HIDE));

    tButtonMenu->setIcon(Tumb::icon(I_MENU));
}

// ------------------------------------------------------
void MainWindow::progressSetValue(int value)
{
    ui->progressBarUpdate->setValue(value);
}

// ------------------------------------------------------
void MainWindow::progressShow(int max)
{
    ui->progressBarUpdate->setRange(0,max);
    ui->progressBarUpdate->setValue(0);
    ui->progressBarUpdate->setVisible(true);
}

// ------------------------------------------------------
void MainWindow::rechargeAlbums()
{
    ui->progressBarUpdate->setVisible(false);
    int cur=ui->comboBoxSwichCat->currentIndex();
    setAlbumPath(cur);
    chargeListItemes();
}

// ------------------------------------------------------
void MainWindow::setAlbumPath(int index)
{
    switch (index) {
    case 0:mMyTreeModel->chargeCategory(COL_I_GENRE,COL_I_ARTIST);           break;
    case 1:mMyTreeModel->chargeCategory(COL_I_GENRE,COL_I_ALBUM);            break;
    case 2:mMyTreeModel->chargeCategory(COL_I_ALBUM,COL_I_ARTIST);           break;
    case 3:mMyTreeModel->chargeCategory(COL_I_ARTIST,COL_I_ALBUM);           break;
    case 4:mMyTreeModel->chargeCategory(COL_I_GENRE,COL_I_ARTIST,COL_I_ALBUM); break;
    case 5:mMyTreeModel->chargeCategory(COL_I_GENRE,COL_I_ALBUM,COL_I_ARTIST); break;
    case 6:mMyTreeModel->chargeCategory(COL_I_ARTIST,COL_I_NULL);            break;
    case 7:mMyTreeModel->chargeCategory(COL_I_ALBUM,COL_I_NULL);             break;

    default: break;
    }
}

// ------------------------------------------------------
QIcon logo(int col){

    switch (col) {
    case COL_I_GENRE: return Tumb::icon(I_Genre);
    case COL_I_ALBUM: return Tumb::icon(I_Album);
    case COL_I_ARTIST:return Tumb::icon(I_ARTIST);
    default:        break;
    }
    return   Tumb::icon(I_Album);
}

// ------------------------------------------------------
void MainWindow::changeStatusPathText()
{

    ui->toolButtonG->setVisible(false);
    ui->toolButtonP->setVisible(false);
    ui->toolButtonU->setVisible(false);

    int id =mMap.value(MAP_ID).toInt();  QString name =mMap.value(MAP_TITLE).toString();
    int pId=mMap.value(MAP_PID).toInt(); QString pName=mMap.value(MAP_PTITLE).toString();
    int gId=mMap.value(MAP_GID).toInt(); QString gName=mMap.value(MAP_GTITLE).toString();

    QFontMetrics fm(this->font());
    QString gTxt=fm.elidedText(gName,Qt::ElideRight,160);
    QString pTxt=fm.elidedText(pName,Qt::ElideRight,160);
    QString nTxt=fm.elidedText(name, Qt::ElideRight,160);

    if(!gName.isEmpty()){
        ui->toolButtonG->setText(gTxt);
        ui->toolButtonG->setIcon(logo(gId));
        ui->toolButtonG->setVisible(true);
    }

    if(!pName.isEmpty()){
        ui->toolButtonP->setText(pTxt);
        ui->toolButtonP->setIcon(logo(pId));
        ui->toolButtonP->setVisible(true);
    }

    if(!name.isEmpty()){
        ui->toolButtonU->setText(nTxt);
        ui->toolButtonU->setIcon(logo(id));
        ui->toolButtonU->setVisible(true);
    }

}

// ------------------------------------------------------
void MainWindow::onTreeAudioActivated(const QModelIndex &index)
{
    QModelIndex idx=mMyListModel->index(index.row(),HIDER_TITLE);
    QString path   =idx.data(Qt::UserRole).toString();
    QString title  =idx.data().toString();
    if(path.isEmpty() )return;

    QList<QVariantMap>list;
    QVariantMap map;

    map["Title"]=title;
    map["Url"]  =path;

    list.append(map);

    mPlayer->addToPlaylist(list);
    mPlayer->playLast();
}

// ------------------------------------------------------
void MainWindow::onTreeAudioClicked(const QModelIndex &index)
{
    if(index.column()==HIDER_ADD){
        QModelIndex idx=mMyListModel->index(index.row(),HIDER_TITLE);

        QString path =idx.data(Qt::UserRole).toString();
        QString title=idx.data().toString();
        if(path.isEmpty() )return;

        QVariantMap map;
        map["Title"]=title;
        map["Url"  ]=path;

        QList<QVariantMap>list;
        list.append(map);
        mPlayer->addToPlaylist(list);

    }else if(index.column()==HIDER_FAVO){
        QModelIndex idx=mMyListModel->index(index.row(),HIDER_FAVO);
        if(!idx.isValid())return;

        bool favo   =idx.data(USER_RATED).toBool();
        QString path=mMyListModel->index(index.row(),HIDER_TITLE).data(Qt::UserRole).toString();
        if (DataBase::setFavorite(path,!favo)){
            mMyListModel->itemFromIndex(index)->setData(!favo,USER_RATED);
            mMyListModel->itemFromIndex(index)->setIcon(favo ? Tumb::icon(I_START):Tumb::icon(I_STARTED));
            //qDebug()<<"mainwindow favorit saved";
        }
    }

}

// ------------------------------------------------------
void MainWindow::onTreeViewContentActivated(const QModelIndex &index)
{
    mSearchBar->setVisible(false);
    QModelIndex  pIndex= index.parent();
    QModelIndex  gIndex= pIndex.parent();
    int isfavo   =index.data(USER_FAVO_DISPLY).toBool();

    mMap[MAP_ID]     =index.data(USER_ID).toInt();
    mMap[MAP_TITLE]  =index.data(USER_TITLE).toString();;
    mMap[MAP_PID]    =pIndex.data(USER_ID).toInt();
    mMap[MAP_PTITLE] =pIndex.data().toString();;
    mMap[MAP_GID]    =gIndex.data(USER_ID).toInt();
    mMap[MAP_GTITLE] =gIndex.data().toString();
    mMap[MAP_CHILD]  =index.data(USER_CHILD_ID).toInt();
    mMap[MAP_IMG]    =index.data(USER_IMGPATH).toString();;
    mMap[MAP_IS_FAVO]=/*QString::number*/(isfavo);

    chargeListItemes();

}

// ------------------------------------------------------
void MainWindow::onListViewActivated(const QModelIndex &index)
{
    mSearchBar->setVisible(false);
    int pId  =mMap.value(MAP_ID).toInt();
    QString pName=mMap.value(MAP_TITLE).toString();
    int gId  =mMap.value(MAP_PID).toInt();
    QString gName=mMap.value(MAP_PTITLE).toString();

    int isfavo=index.data(USER_FAVO_DISPLY).toBool();

    mMap[MAP_ID]       =index.data(USER_ID).toInt();
    mMap[MAP_TITLE]    =index.data(USER_TITLE).toString();
    mMap[MAP_PID]      =pId;
    mMap[MAP_PTITLE]   =pName;
    mMap[MAP_GID]      =gId;
    mMap[MAP_GTITLE]   =gName;
    mMap[MAP_CHILD]    =/*QString::number*/(COL_I_NULL);
    mMap[MAP_IMG]      =index.data(USER_IMGPATH).toString();
    mMap[MAP_IS_FAVO]  =/*QString::number*/(isfavo);

    chargeListItemes();

}

// ------------------------------------------------------
void MainWindow::chargeListItemes()
{
    int id=     mMap.value(MAP_ID).toInt();     QString name=   mMap.value(MAP_TITLE).toString();
    int pId=    mMap.value(MAP_PID).toInt();    QString pName=  mMap.value(MAP_PTITLE).toString();
    int gId=    mMap.value(MAP_GID).toInt();    QString gName=  mMap.value(MAP_GTITLE).toString();
    int chld=   mMap.value(MAP_CHILD).toInt();  QString path=   mMap.value(MAP_IMG).toString();
    bool isFavo=mMap.value(MAP_IS_FAVO).toInt();

    //  qDebug()<<"album rated"<<isFavo<<id<<chld<<pId<<pName;

    if(chld!=COL_I_NULL){
        if(isFavo)
            mMyListModel->chargeFavoritedAlbum();
        else
            mMyListModel->chargeCategory(name,id,chld,pId,pName);

        ui->stackedWidgetView->setCurrentWidget(ui->page_View);
        changeStatusPathText();
        mMyListModel->sort(0,Qt::AscendingOrder);
        return;
    }

    if(isFavo){

        if(id==COL_I_ALBUM)
            mMyListModel->chargeAudios(name,COL_I_ALBUM);
        else
            mMyListModel->chargeAudios("1",id);

    } else   {
        mMyListModel->chargeAudios(name,id,pName,pId,gName,gId);
    }

    if(isFavo && id!=COL_I_ALBUM){
        mTVAudio->setColumnHidden(HIDER_ARTIST,true);
        mTVAudio->setColumnHidden(HIDER_ALBUM,true);
    }else{
        mTVAudio->setColumnHidden(HIDER_ALBUM,id==COL_I_ALBUM);
        mTVAudio->setColumnHidden(HIDER_ARTIST,id!=COL_I_ALBUM);
    }

    ui->tb_favoritAlbum->setVisible(id==COL_I_ALBUM);
    ui->tb_imgAlbum->setVisible(!isFavo);
    setlabelImage();

    mTVAudio->resizeColumnToContents(HIDER_TITLE);
    mTVAudio->resizeColumnToContents(HIDER_TIME);

    ui->stackedWidgetView->setCurrentWidget(ui->pageAudio);

    mTVAudio->setColumnWidth(HIDER_ADD,20);
    mTVAudio->setColumnWidth(HIDER_FAVO,20);

    mTVAudio->header()->setSectionResizeMode(HIDER_TITLE,QHeaderView::Stretch);
    mTVAudio->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    changeStatusPathText();

    int favo= Setting::albumIsFavorited(name);
    ui->tb_favoritAlbum->setChecked(favo);
    ui->tb_favoritAlbum->setIcon(favo ? Tumb::icon(I_STARTED):Tumb::icon(I_START));

}

// ------------------------------------------------------
void MainWindow::searchAudios(int col,const QString &text)
{
    mMyListModel->searchAudios(col,text);
    mTVAudio->setColumnHidden(HIDER_ARTIST,false);
    mTVAudio->setColumnHidden(HIDER_ALBUM,false);

    mTVAudio->resizeColumnToContents(HIDER_TITLE);
    mTVAudio->resizeColumnToContents(HIDER_TIME);

    ui->stackedWidgetView->setCurrentWidget(ui->pageAudio);

    mTVAudio->setColumnWidth(HIDER_ADD,20);
    mTVAudio->setColumnWidth(HIDER_FAVO,20);

    mTVAudio->header()->setSectionResizeMode(HIDER_TITLE,QHeaderView::Stretch);
    mTVAudio->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tb_favoritAlbum->setVisible(false);
    ui->tb_imgAlbum->setVisible(false);
    // changeStatusPathText();
    QIcon icon(":/icons/find");
    ui->labelAlbumImage->setPixmap(icon.pixmap(mIconSize,mIconSize));

}

// ------------------------------------------------------
void MainWindow::setlabelImage()
{
    int       id=   mMap.value(MAP_ID).toInt();
    QString name=   mMap.value(MAP_TITLE).toString();
    QString path=   mMap.value(MAP_IMG).toString();
    bool  isFavo=   mMap.value(MAP_IS_FAVO).toInt();
    QIcon img;

    if(mSearchBar->isVisible()) { img= QIcon (":/icons/find");}
    else if(isFavo && id!=COL_I_ALBUM){ img=Tumb::icon(I_FAVO);        }
    else if(id==COL_I_ALBUM)     { img=Tumb::iconAlbum(name,path);}
    else  if(id==COL_I_ARTIST)    { img=Tumb::iconnArtist(name);   }

    ui->labelAlbumImage->setPixmap(QPixmap(img.pixmap(mIconSize,mIconSize)));

}

// ------------------------------------------------------
void MainWindow::on_tb_paneContent_toggled(bool checked)
{
    ui->tb_paneContent->setIcon(checked ? Tumb::icon(I_PANE_HIDE):Tumb::icon(I_PANE_SHOW ));
    ui->widgetContent->setVisible(!checked);
}

// ------------------------------------------------------
void MainWindow::on_tb_panePlaylist_toggled(bool checked)
{

    ui->tb_panePlaylist->setIcon(checked ? Tumb::icon(I_PANE_SHOW):Tumb::icon(I_PANE_HIDE));
    mWPlayList->setVisible(!checked);

}

// ------------------------------------------------------
void MainWindow::on_tb_favoritAlbum_clicked()
{

    QString Name=   mMap.value(MAP_TITLE).toString();
    bool favo=ui->tb_favoritAlbum->isChecked();

    ui->tb_favoritAlbum->setIcon(favo ? Tumb::icon(I_STARTED):Tumb::icon(I_START));

    Setting::setAlbumFavo(Name,favo);
}

// ------------------------------------------------------
void MainWindow::on_tb_addAlbum_clicked()
{


    QList<QVariantMap>list ;
    QVariantMap map;
    int count=  mTVAudio->model()->rowCount();

    for (int i = 0; i < count; ++i) {

        QModelIndex idx=mTVAudio->model()->index(i,HIDER_TITLE);
        QString title  =idx.data().toString();
        QString path   =idx.data(Qt::UserRole).toString();

        map["Title"]=title;
        map["Url"]  =path;

        list.append(map);

    }

    mPlayer->addToPlaylist(list);

}

// ------------------------------------------------------
void MainWindow::on_tb_playAlbum_clicked()
{

    mPlayer->cleanList();
    on_tb_addAlbum_clicked();
    mPlayer->play();
}

// ------------------------------------------------------
void MainWindow::on_tb_imgAlbum_clicked()
{
    QString Name=   mMap.value(MAP_TITLE).toString();
    qDebug()<<"MainWindow::on_tb_imgAlbum_clicked:::"<<Name;

    if(Name==tr("Unknown")||Name.isEmpty()) return;

    int id=   mMap.value(MAP_ID).toInt();

    changeImageAlbum(id,Name);

}

// ------------------------------------------------------
void MainWindow::onChangeImageAlbum(int row)
{

    QModelIndex index=mMyListModel->index(row,0);
    if(!index.isValid())return;


    QString  Name=index.data(USER_TITLE).toString();
    if(Name==tr("Unknown")) return;

    int      Id=index.data(USER_ID).toInt();

    changeImageAlbum(Id,Name);

}

// ------------------------------------------------------
void MainWindow::changeImageAlbum(int id ,const QString Name)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    D_Picture,
                                                    "Images (*.png *.xpm *.jpg)");

    if(fileName.isEmpty()) return;

    QImage img(fileName);

    img= img.scaled(256,256,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    if(id==COL_I_ALBUM)
        img.save(CACHE_ALBUM+"/"+Name+".jpg","jpg");
    else if(id==COL_I_ARTIST)
        img.save(CACHE_ARTIST+"/"+Name+".jpg","jpg");

}

// ------------------------------------------------------
void MainWindow::onFavoritAlbum(int row,bool favo)
{

    QModelIndex index=mMyListModel->index(row,0);
    if(!index.isValid())return;

    bool oldfavo=index.data(USER_RATED).toBool();
    // qDebug()<<oldfavo<<favo;
    if(oldfavo==favo)return;

    Setting::setAlbumFavo(index.data(USER_TITLE).toString(),favo);
    mMyListModel->itemFromIndex(index)->setData(favo,USER_RATED);
    // qDebug()<<row<<favo;

}

// ------------------------------------------------------
void MainWindow::onAddAlbum(int row)
{
    //qDebug()<<row<<"onAddAlbumClicked";
    QModelIndex index=mMyListModel->index(row,0);

    if(!index.isValid())return;

    int      Id=index.data(USER_ID).toInt();
    QString  Name=index.data(USER_TITLE).toString();

    int     pId=    mMap.value(MAP_PID).toInt();
    QString pName=  mMap.value(MAP_PTITLE).toString();

    int     gId=    mMap.value(MAP_GID).toInt();
    QString gName=  mMap.value(MAP_GTITLE).toString();

    if(Name.isEmpty() || Id==-1)return;

    QList<QVariantMap>list = DataBase::getAlbumUrls(Name,Id,pName,pId,gName,gId);

    mPlayer->addToPlaylist(list);


}

// ------------------------------------------------------
void MainWindow::onPlayAlbum(int row)
{
    mPlayer->cleanList();
    onAddAlbum( row);
    mPlayer->play();
}


// ------------------------------------------------------
void MainWindow::onActionopentriggered()
{


    DialogOpen *dlg=new DialogOpen(this);
    if(dlg->exec()==QDialog::Accepted){
        bool save=dlg->autoSave();

        QList<QUrl>list=dlg->listUrls();
        //  qDebug()<<"MainWindow urls :"<<list;
        if(save)
            mMediaUpdate->addFiles(list);

        mPlayer->addToPlaylist(list);
        mPlayer->playLast();
    }
    delete dlg;
}



void MainWindow::setUrl(const QString &file)
{
      qDebug()<<"MainWindow::setUrl:"<<file;
    mPlayer->setFile(file);
    switchViewMode(true);
}


// ------------------------------------------------------
void MainWindow::on_tButtonCancelMsg_clicked()
{
    mMediaUpdate->setUpdateDirs(false);
    ui->widgetMessage->setVisible(false);
}

// ------------------------------------------------------
void MainWindow::on_tButtonOkMsg_clicked()
{
    mMediaUpdate->setUpdateDirs(true);
    ui->widgetMessage->setVisible(false);
}
//----------------------------------- TRAY ICON ---------------------------------------
void MainWindow:: creatTrayIcon()
{
       if(!QSystemTrayIcon::isSystemTrayAvailable())return;
    if(!mTrayIcon){

    mTrayIcon= new QSystemTrayIcon(/*QIcon::fromTheme("goldfinch",*/QIcon(":/icons/goldfinch"));

    //  trayIcon= new QSystemTrayIcon;
        QMenu *trayIconMenu=new QMenu;

        trayIconMenu->addAction(ACtions::PlayAct());
        trayIconMenu->addAction(ACtions::PauseAct());
        trayIconMenu->addAction(ACtions::NextAct());
        trayIconMenu->addAction(ACtions::PrevAct());
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(ACtions::quitAct());



        mTrayIcon->setContextMenu(trayIconMenu);
        connect(mTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
        mTrayIcon->show();
    }else{
          mTrayIcon->show();
    }


}


//----------------------------------------------------------------------------------
void MainWindow::setwTitle(const QString &title,const QString &info)
{
    setWindowTitle(title);

    if(mTitle==title)return;
       mTitle=title;

    if(!mShowNotification)return;

  if(windowState()==Qt::WindowMinimized  || isHidden())
     qDebug()<<windowState();
  else   return;

#ifdef Q_OS_UNIX
        // TODO  :FIX NOTIFICATION
    QString tumbcach=TEMP_CACH+"/"+title+".png";
    if(!QFile::exists(tumbcach))return;
    qDebug()<<"tumbcach"<<tumbcach;
    if(PlayerAdaptor::Notify( QApplication::applicationName(),
                              tumbcach,
                              title,info,  -1))
    {   return;    }

#endif

    if(QSystemTrayIcon::supportsMessages()){
        QIcon ico=QIcon::fromTheme("goldfinch",QIcon(":/icons/goldfinch"));
        if(mTrayIcon)
            mTrayIcon->showMessage(tr("Goldfinch"),title+"\n"+info,ico);

    }

}
//----------------------------------------------------------------------------------
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger: isHidden()?showRaise():hide(); break;
    case QSystemTrayIcon::DoubleClick: break;
    case QSystemTrayIcon::MiddleClick: break;
    default: break  ;
    }
}

void MainWindow::showPropertyDialog(bool isRead,const QString &file)
{

    PropertiesFile *dlg=new PropertiesFile(file,isRead);
     dlg->exec();
    delete dlg;


}

void MainWindow::showSettings()
{
    DialogOptions *dlg=new DialogOptions;
    if(dlg->exec()==QDialog::Accepted){
        //
        bool isClear=dlg->clearData();
        if(isClear){
            if(DataBase::clearDatabase()){
                QFile::remove(CACHE+"/filesinfo");
                rechargeAlbums();
            }
        }
        //
        mMediaUpdate->addUpdateDirectories(false);
    }
    QSettings settings;
    settings.beginGroup("Window");
    mShowNotification=settings.value("ShowNotification",false).toBool();
    bool trayicon=settings.value("TrayIcon",true).toBool();
    if(trayicon) {    creatTrayIcon();
    }else{if(mTrayIcon) mTrayIcon->hide(); }

    settings.endGroup();

    delete dlg;
}
