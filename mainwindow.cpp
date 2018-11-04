#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tumb.h"
//#include "dialogoptions.h"
//#include <QDebug>
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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mMyTreeModel=new MyContentModel;
    mMyListModel=new MyListModel;
    mMediaUpdate=new MediaUpdate;
    mTVContent=new TreeViewContent;
    mListView=new ListView;
    mTVAudio=new TreeViewAudio;
    mLIDelegate=new ListItemDelegate;
    mTItemDelegate=new TreeItemDelegate;
    mWPlayList=new WidgetPlayList;
    mPlayer=new Player(mWPlayList->playListView(),this);
    mImageInfo=new WidgetImageInfo;
    mSearchBar=new SearchBar(this);


   //-- Slider Change Icons Size
    mSliderIconSize = new QSlider(Qt::Horizontal, this);
    mSliderIconSize->setRange(96,256);
    mSliderIconSize->setMaximumWidth(160);
    ui->statusBar->addPermanentWidget(mSliderIconSize);


    //-- Acton Switch view mode
    mActSwich=new QAction(tr("Mini player"),this);
    mActSwich->setShortcut(QKeySequence("Ctrl+M"));
    mActSwich->setCheckable(true);
    connect(mActSwich,SIGNAL(triggered(bool)),this,SLOT(switchViewMode(bool)));

    //-- ACtion Search
    QAction *actSearch=new QAction(Tumb::icon(I_FIND),tr("Search"),this);
    actSearch->setShortcut(QKeySequence("Ctrl+F"));
   // actSearch->setCheckable(true);
    connect(actSearch,&QAction::triggered,mSearchBar,&SearchBar::showHid);

    //--  Menu Main
    QMenu *menu=new QMenu;
    menu->addAction(tr("Open Foles..."),this,&MainWindow::onActionopentriggered);
    menu->addAction(tr("Add/Update Directory"),mMediaUpdate,&MediaUpdate::getDirListOptions);
    menu->addAction(actSearch);
    menu->addAction(mActSwich);
    menu->addSeparator();
    menu->addAction(Tumb::icon(I_QUIT),tr("Quit"),this,&MainWindow::close);
    ui->tButtonMenu->setMenu(menu);

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

    ui->hLayoutControle->insertWidget(0,mPlayer);

    ui->vLayoutViewAll->insertWidget(0,mSearchBar);

    ui->progressBarUpdate->setVisible(false);

    ui->widgetMessage->setVisible(false);

    mSearchBar->setVisible(false);



    //   ------------------------        CONNECTIONS      ------------------------

    connect(mWPlayList,  &WidgetPlayList::rmovePlaylistItem,  mPlayer,    &Player::rmovePlaylistItem);
    connect(mWPlayList,  &WidgetPlayList::movCurentItem,      mPlayer,    &Player::moveMedia);
    connect(mWPlayList,  &WidgetPlayList::cleanList,          mPlayer,    &Player::cleanList);
    connect(mWPlayList,  &WidgetPlayList::playbackModeChanged,mPlayer,    &Player::setPlaybackMode);

    connect(mPlayer,     &Player::imageChanged,               mImageInfo,  &WidgetImageInfo::setImage);
    connect(mPlayer,     &Player::titleChanged,               mImageInfo,  &WidgetImageInfo::setTitle);
    connect(mPlayer,     &Player::infoChanged,                mImageInfo,  &WidgetImageInfo::setInfo);
    connect(mPlayer,     &Player::updateSong,                 mMediaUpdate,&MediaUpdate::updateFile);
    connect(mPlayer,     &Player::titleChanged,               this      ,  &MainWindow::setwTitle);

    connect(this,        &MainWindow::iconsChanged,           mPlayer,     &Player::iconsChanged);
    connect(this,        &MainWindow::iconsChanged,           mWPlayList,  &WidgetPlayList::setupIcons);

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

    connect(mListView,        &ListView::activated,           this,&MainWindow::onListViewActivated);
    connect(mSliderIconSize,  &QSlider::valueChanged,          this,&MainWindow::setIconSize);


    connect(mPlayer,&Player::playBackChanged,this->ui->statusBar,&QStatusBar::showMessage);
    //connect(ui->actionaddDir, &QAction::triggered,  mMediaUpdate,&MediaUpdate::getDirListOptions);
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

        ui->stackedWidget->setVisible(false);
        ui->statusBar->setVisible(false);
        ui->vLayout_Center->addWidget(mImageInfo);
        mImageInfo->setHorizontal(false);
        mImageInfo->setMinimumSize(QSize(0,0));
        showNormal();
        resize(360,400);
        adjustSize();
        resize(360,400);
        //
    }else{
        restoreGeometry(settings.value("Geometry").toByteArray());

        ui->stackedWidget->setVisible(true);
        ui->statusBar->setVisible(true);
        ui->hLayoutControle->insertWidget(0,mImageInfo);
        mImageInfo->setMinimumSize(QSize(200,50));
        mImageInfo->setHorizontal(true);

    }

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

    // QString file=D_CACHE+"/playlist";
    QSettings settings;
    settings.beginGroup("Recent");

    settings.setValue("Index", ui->comboBoxSwichCat->currentIndex());
    settings.setValue(MAP_ID,     mMap.value(MAP_ID));
    settings.setValue(MAP_PID,    mMap.value(MAP_PID));
    settings.setValue(MAP_GID,    mMap.value(MAP_GID));
    settings.setValue(MAP_CHILD,  mMap.value(MAP_CHILD));
    settings.setValue(MAP_TITLE,  mMap.value(MAP_TITLE));
    settings.setValue(MAP_PTITLE, mMap.value(MAP_PTITLE));
    settings.setValue(MAP_GTITLE, mMap.value(MAP_GTITLE));
    settings.setValue(MAP_IMG,    mMap.value(MAP_IMG));
    settings.setValue(MAP_IS_FAVO,mMap.value(MAP_IS_FAVO));

    settings.endGroup();

    settings.beginGroup("Window");

    if(!mActSwich->isChecked()){
        settings.setValue("Geometry",this->saveGeometry());
        settings.setValue("SState",ui->splitter->saveState());
    }
    settings.setValue("MiniPlayer",mActSwich->isChecked());
    settings.setValue("IconSize",mIconSize);

    settings.endGroup();

    delete ui;

}

// ------------------------------------------------------
void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::StyleChange)
    {
        changeStyleSheet();
    }
}

// ------------------------------------------------------
void MainWindow::changeStyleSheet()
{

    QColor col=this->palette().text().color();
    QColor coled=this->palette().highlight().color();
    if (mcoloor==col.name()+coled.name())
        return;

    mcoloor=col.name()+coled.name();
    //Tumb::setIconColor(col,coled);
    QString leftBorder , rightBorder;

    if(isRightToLeft()){
        leftBorder="border-top-right-radius: 6px;"
                   "border-bottom-right-radius: 6px;";
        rightBorder="border-top-left-radius: 6px;"
                    "border-bottom-left-radius: 6px;";
    }else{
        leftBorder="border-top-left-radius: 6px;"
                   "border-bottom-left-radius: 6px;";
        rightBorder="border-top-right-radius: 6px;"
                    "border-bottom-right-radius: 6px;";

    }

    int h=mSearchBar->height()/2;
    int tb_h=h-2;

    QString style=QString(

                /*-- search bar --*/
                "QWidget#SearchBar{"
                "   background-color: palette(base);"
                "   border:1px solid palette(highlight);"
                "   border-radius:%3px;"
                "}"
                "QToolButton#TBSearch{"
                 "   background-color: palette(base);"
                "    border:0px ;"
                "   border-radius: %4px;"
                "}"
                "QToolButton#TBSearch:hover{ background-color: palette(shadow);}"
                "QToolButton#TBSearch:pressed{background-color: palette(highlight);}"
                 /*-- TreeView ListView --*/
                "QTreeView{"
                "background-color: transparent;"
                //   "border: 1px solid palette(midlight);"
                // "border-color: palette(highlight);"
                "color:palette(window-text);"
                "}"
                " QListView{"
                " background-color: transparent;"
                " border-color: palette(Window);"
                "color:palette(window-text);"
                " }"
                /*-- Player Control --*/
                "QToolButton#PrevButton{"
                " border: 1px solid palette(shadow);"
                "%1"
                "}"
                "QToolButton#PlayButton{"
                " border: 1px solid palette(shadow);"
                "border-left-color:transparent;"
                "border-right-color:transparent;"
                "}"
                "QToolButton#NextButton{"
                " border: 1px solid palette(shadow);"
                "%2"
                "}"
                "QToolButton#PrevButton:hover,#PlayButton:hover,#NextButton:hover{"
                "background-color:palette(highlight)"
                "}"
                "QToolButton#PrevButton:pressed,#PlayButton:pressed,#NextButton:pressed{"
                "background-color:palette(shadow)"
                "}"
                "QToolButton#VolumeButton ,#tButtonMenu{"
                "border-radius: 15px;"
                "width: 30px;"
                "height:30px;"
                " max-width: 30px;"
                " max-height:30px;"
                "}"
                "QToolButton#VolumeButton:hover ,#tButtonMenu:hover{"
                // "border: 1px solid palette(highlight);"
                "background-color:palette(shadow)"
                "}"
                "QToolButton#VolumeButton:pressed ,#tButtonMenu:pressed{"
                // "border: 1px solid palette(shadow);"
                "background-color:palette(highlight)"
                "}").arg(leftBorder).arg(rightBorder).arg(h).arg(tb_h);

    setStyleSheet(style) ;

    setupIcons();
    mWPlayList->setupIcons();
    emit iconsChanged();
}

// ------------------------------------------------------
void MainWindow::chargeRecent()
{
    // mMyTreeModel->chargeCategory(CAT_GENRE,CAT_ALBUM);
    QString file=D_CACHE+"/playlist";
    QSettings settings;

    settings.beginGroup("Recent");

    mMap[MAP_ID]     =settings.value(MAP_ID,0).toString();
    mMap[MAP_PID]    =settings.value(MAP_PID,0).toString();
    mMap[MAP_GID]    =settings.value(MAP_GID,0).toString();
    mMap[MAP_CHILD]  =settings.value(MAP_CHILD,0).toString();
    mMap[MAP_TITLE]  =settings.value(MAP_TITLE,0).toString();
    mMap[MAP_PTITLE] =settings.value(MAP_PTITLE,0).toString();
    mMap[MAP_GTITLE] =settings.value(MAP_GTITLE,0).toString();
    mMap[MAP_IMG]    =settings.value(MAP_IMG,0).toString();
    mMap[MAP_IS_FAVO]=settings.value(MAP_IS_FAVO,0).toString();
    int index    =settings.value("Index",0).toInt();

    settings.endGroup();

    settings.beginGroup("Window");
    restoreGeometry(settings.value("Geometry").toByteArray());
    ui->splitter->restoreState(settings.value("SState").toByteArray());
    mIconSize=settings.value("IconSize",128).toInt();
    mSliderIconSize->setValue(mIconSize);
    bool trayicon=settings.value("TrayIcon",true).toBool();
    bool mini=settings.value("MiniPlayer",false).toBool();

    settings.endGroup();

    ui->comboBoxSwichCat->setCurrentIndex(index);
    setIconSize(mIconSize);
    setAlbumPath(index);
    chargeListItemes();
    if(trayicon &&  QSystemTrayIcon::isSystemTrayAvailable())
        creatTrayIcon();

    mActSwich->setChecked(mini);
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
    ui->tButtonMenu->setIcon(Tumb::icon(I_MENU));
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
QString logo(int col){

    switch (col) {
    case COL_I_GENRE: return ":/icons/genre-16";
    case COL_I_ALBUM: return ":/icons/cover-album";
    case COL_I_ARTIST:return ":/icons/artist-16";
    default:        break;
    }
    return   ":/icons/star";
}

// ------------------------------------------------------
void MainWindow::changeStatusPathText()
{

    ui->labelStatusG->clear();
    ui->labelStatusP->clear();
    ui->labelStatus->clear();

    int id =mMap.value(MAP_ID).toInt();  QString name =mMap.value(MAP_TITLE);
    int pId=mMap.value(MAP_PID).toInt(); QString pName=mMap.value(MAP_PTITLE);
    int gId=mMap.value(MAP_GID).toInt(); QString gName=mMap.value(MAP_GTITLE);

    QFontMetrics fm(this->font());
    QString gTxt=fm.elidedText(gName,Qt::ElideRight,200);
    QString pTxt=fm.elidedText(pName,Qt::ElideRight,200);
    QString nTxt=fm.elidedText(name, Qt::ElideRight,200);


    if(!gName.isEmpty()){
        ui->labelStatusG->setText( QString("<html><head/><body><p><img src='%1'/> %2 </p></body></html>")
                                   .arg(logo(gId)).arg(gTxt));
    }

    if(!pName.isEmpty()){
        ui->labelStatusP->setText( QString("<html><head/><body><p><img src='%1'/> %2 </p></body></html>")
                                   .arg(logo(pId)).arg(pTxt));
    }
    if(!name.isEmpty()){
        ui->labelStatus->setText(QString("<html><head/><body><p><img src='%1'/> %2 </p></body></html>")
                                 .arg(logo(id)).arg(nTxt));
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

    mMap[MAP_ID]     =index.data(USER_ID).toString();
    mMap[MAP_TITLE]  =index.data(USER_TITLE).toString();;
    mMap[MAP_PID]    =pIndex.data(USER_ID).toString();
    mMap[MAP_PTITLE] =pIndex.data().toString();;
    mMap[MAP_GID]    =gIndex.data(USER_ID).toString();
    mMap[MAP_GTITLE] =gIndex.data().toString();
    mMap[MAP_CHILD]  =index.data(USER_CHILD_ID).toString();;
    mMap[MAP_IMG]    =index.data(USER_IMGPATH).toString();;
    mMap[MAP_IS_FAVO]=QString::number(isfavo);

    chargeListItemes();

}

// ------------------------------------------------------
void MainWindow::onListViewActivated(const QModelIndex &index)
{
mSearchBar->setVisible(false);
    QString pId  =mMap.value(MAP_ID);
    QString pName=mMap.value(MAP_TITLE);
    QString gId  =mMap.value(MAP_PID);
    QString gName=mMap.value(MAP_PTITLE);

    int isfavo=index.data(USER_FAVO_DISPLY).toBool();

    mMap[MAP_ID]       =index.data(USER_ID).toString();
    mMap[MAP_TITLE]    =index.data(USER_TITLE).toString();
    mMap[MAP_PID]      =pId;
    mMap[MAP_PTITLE]   =pName;
    mMap[MAP_GID]      =gId;
    mMap[MAP_GTITLE]   =gName;
    mMap[MAP_CHILD]    =QString::number(COL_I_NULL);
    mMap[MAP_IMG]      =index.data(USER_IMGPATH).toString();
    mMap[MAP_IS_FAVO]  =QString::number(isfavo);

    chargeListItemes();

}

// ------------------------------------------------------
void MainWindow::chargeListItemes()
{
    int id=     mMap.value(MAP_ID).toInt();     QString name=   mMap.value(MAP_TITLE);
    int pId=    mMap.value(MAP_PID).toInt();    QString pName=  mMap.value(MAP_PTITLE);
    int gId=    mMap.value(MAP_GID).toInt();    QString gName=  mMap.value(MAP_GTITLE);
    int chld=   mMap.value(MAP_CHILD).toInt();  QString path=   mMap.value(MAP_IMG);
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
    QString name=   mMap.value(MAP_TITLE);
    QString path=   mMap.value(MAP_IMG);
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

    QString Name=   mMap.value(MAP_TITLE);
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
    QString Name=   mMap.value(MAP_TITLE);
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
        img.save(D_ALBUM_CACHE+"/"+Name+".jpg","jpg");
    else if(id==COL_I_ARTIST)
        img.save(D_ARTIST_CACHE+"/"+Name+".jpg","jpg");

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
    QString pName=  mMap.value(MAP_PTITLE);

    int     gId=    mMap.value(MAP_GID).toInt();
    QString gName=  mMap.value(MAP_GTITLE);

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
        bool save=dlg->autoSave(); //TODO :add this to save

        QList<QUrl>list=dlg->listUrls();
        //  qDebug()<<"MainWindow urls :"<<list;
        if(save)
            mMediaUpdate->addFiles(list);

        mPlayer->addToPlaylist(list);
        mPlayer->playLast();
    }
    delete dlg;
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
    if(!trayIcon){
        trayIcon= new QSystemTrayIcon(QIcon::fromTheme("goldfinch",QIcon(":/icons/goldfinch-24")));
        QMenu *trayIconMenu=new QMenu;
        trayIconMenu->addAction(Tumb::icon(I_PLAY), tr("Play"),    mPlayer,&Player::play);
        trayIconMenu->addAction(Tumb::icon(I_PAUSE),tr("Pause"),   mPlayer,&Player::pause);
        trayIconMenu->addAction(Tumb::icon(I_NEXT), tr("Next"),    mPlayer,&Player::next);
        trayIconMenu->addAction(Tumb::icon(I_PREV), tr("Previous"),mPlayer,&Player::previous);
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(Tumb::icon(I_PLAY), tr("Quit"),    this,&MainWindow::close);
        trayIcon->setContextMenu(trayIconMenu);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    }
    trayIcon->show();

}

//----------------------------------------------------------------------------------
void MainWindow::setwTitle(const QString &title)
{
    setWindowTitle(title);
    if(isHidden()){
        QIcon ico=QIcon::fromTheme("goldfinch",QIcon(":/icons/goldfinch"));
        if(trayIcon)
            trayIcon->showMessage(QApplication::applicationDisplayName(),title,ico);

    }
}

//----------------------------------------------------------------------------------
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger: isHidden()?show():hide(); break;
    case QSystemTrayIcon::DoubleClick: break;
    case QSystemTrayIcon::MiddleClick: break;
    default: break  ;
    }
}
