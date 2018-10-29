#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tumb.h"
#include "dialogoptions.h"
#include <QDebug>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QColor col=this->palette().text().color();
//    QColor coled=this->palette().highlight().color();
    //Tumb::setIconColor(col,coled);

    //setAnimated(true);
  //  mSetting=new Setting;
    //  mDataBase=new DataBase;
    mMyTreeModel=new MyContentModel;
    mMyListModel=new MyListModel;
    mMediaUpdate=new MediaUpdate;
    mTVContent=new TreeViewContent;
    mListView=new ListView;
    mTVAudio=new TreeViewAudio;
    mLIDelegate=new ListItemDelegate;
    mTItemDelegate=new TreeItemDelegate;
    mWPlayList=new WidgetPlayList;
    mPlayer=new Player(mWPlayList->playListView());
    mImageInfo=new WidgetImageInfo;

    // Slider Change Icons Size
    mSliderIconSize = new QSlider(Qt::Horizontal, this);
    mSliderIconSize->setRange(96,256);
    mSliderIconSize->setMaximumWidth(160);
    ui->statusBar->addPermanentWidget(mSliderIconSize);


    //-- Acton Switch view mode
    mActSwich=new QAction(tr("Mini player"),this);
    mActSwich->setShortcut(QKeySequence("Ctrl+M"));
    mActSwich->setCheckable(true);
    connect(mActSwich,SIGNAL(triggered(bool)),this,SLOT(switchViewMode(bool)));
    // addAction(mActSwich);

    //--  Menu Main
    QMenu *menu=new QMenu;
    menu->addAction(ui->actionopen);
    menu->addAction(ui->actionaddDir);
    menu->addAction(mActSwich);
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

    ui->progressBarUpdate->setVisible(false);

    ui->widgetMessage->setVisible(false);


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
    connect(mMediaUpdate,&MediaUpdate::dirNidUpdate,          this->ui->widgetMessage, &QWidget::setVisible);

    connect(mLIDelegate, &ListItemDelegate::addAlbum,         this, &MainWindow::onAddAlbum);
    connect(mLIDelegate, &ListItemDelegate::playAlbum,        this, &MainWindow::onPlayAlbum);
    connect(mLIDelegate, &ListItemDelegate::favoritAlbum,     this, &MainWindow::onFavoritAlbum);
    connect(mLIDelegate, &ListItemDelegate::changeImageAlbum, this, &MainWindow::onChangeImageAlbum);

    connect(mTVContent,  &TreeViewContent::activated,         this, &MainWindow::treeViewContentActivated);
    connect(mTVContent,  &TreeViewContent::clicked,           this, &MainWindow::treeViewContentActivated);

    connect(mTVAudio,    &TreeViewAudio::clicked,             this, &MainWindow::treeAudioClicked);
    connect(mTVAudio,    &TreeViewAudio::treeAudioClicked,    this, &MainWindow::treeAudioClicked);
    connect(mTVAudio,    &TreeViewAudio::updateCurent,        this, &MainWindow::chargeListItemes);
    connect(mTVAudio,    &TreeViewAudio::activated,           this, &MainWindow::treeAudioActivated);

    connect(mListView,        &ListView::activated,           this,&MainWindow::listViewActivated);
    connect(mSliderIconSize,  &QSlider::valueChanged,          this,&MainWindow::setIconSize);

    //   connect(this->ui->comboBoxSwichCat,&QComboBox::currentIndexChanged,this,&MainWindow::setPathAlbum);
    connect(ui->comboBoxSwichCat,SIGNAL(currentIndexChanged(int)),this,SLOT(setPathAlbum(int)));
    // connect(mPlayer,SIGNAL(playBackChanged(QString)),ui->statusBar,SLOT(showMessage(QString)));
    connect(mPlayer,     &Player::playBackChanged,            this->ui->statusBar,&QStatusBar::showMessage);

    DataBase::openDataBase();

    switchViewMode(false);

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
        resize(300,320);
        adjustSize();
        resize(300,320);
        //
    }else{
        restoreGeometry(settings.value("Geometry").toByteArray());

        ui->stackedWidget->setVisible(true);
        ui->statusBar->setVisible(true);
        ui->hLayoutControle->insertWidget(0,mImageInfo);
        mImageInfo->setMinimumSize(QSize(190,50));
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

    settings.setValue("Geometry",this->saveGeometry());
    settings.setValue("SState",ui->splitter->saveState());
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
    QString style=QString("QTreeView{"
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
                          "max-width: 30px;"
                          " max-height:30px;"
                          "}"
                          "QToolButton#VolumeButton:hover ,#tButtonMenu:hover{"
                          // "border: 1px solid palette(highlight);"
                          "background-color:palette(shadow)"
                          "}"
                          "QToolButton#VolumeButton:pressed ,#tButtonMenu:pressed{"
                          // "border: 1px solid palette(shadow);"
                          "background-color:palette(highlight)"
                          "}").arg(leftBorder).arg(rightBorder);
    //::menu-button
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
    settings.endGroup();

    ui->comboBoxSwichCat->setCurrentIndex(index);
    setIconSize(mIconSize);
    setPathAlbum(index);
    chargeListItemes();
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
void MainWindow::on_actionaddDir_triggered()
{
    //    QString path="/media/Data/Music";
    //    mMediaUpdate->addDirectory(path);

    DialogOptions *dlg=new DialogOptions;
    if(dlg->exec()==QDialog::Accepted){
        mMediaUpdate->addUpdateDirectory();
    }
    delete dlg;
}

// ------------------------------------------------------
void MainWindow::rechargeAlbums()
{
    ui->progressBarUpdate->setVisible(false);
    int cur=ui->comboBoxSwichCat->currentIndex();
    setPathAlbum(cur);
}

// ------------------------------------------------------
void MainWindow::setPathAlbum(int index)
{
    switch (index) {
    case 0:mMyTreeModel->chargeCategory(CAT_GENRE,CAT_ARTIST);           break;
    case 1:mMyTreeModel->chargeCategory(CAT_GENRE,CAT_ALBUM);            break;
    case 2:mMyTreeModel->chargeCategory(CAT_ALBUM,CAT_ARTIST);           break;
    case 3:mMyTreeModel->chargeCategory(CAT_ARTIST,CAT_ALBUM);           break;
    case 4:mMyTreeModel->chargeCategory(CAT_GENRE,CAT_ARTIST,CAT_ALBUM); break;
    case 5:mMyTreeModel->chargeCategory(CAT_GENRE,CAT_ALBUM,CAT_ARTIST); break;
    case 6:mMyTreeModel->chargeCategory(CAT_ARTIST,CAT_NULL);            break;
    case 7:mMyTreeModel->chargeCategory(CAT_ALBUM,CAT_NULL);             break;

    default: break;
    }
}

// ------------------------------------------------------
QString logo(int col){

    switch (col) {
    case CAT_GENRE: return ":/icons/genre-16";
    case CAT_ALBUM: return ":/icons/cover-16";
    case CAT_ARTIST:return ":/icons/artist-16";
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
void MainWindow::treeAudioActivated(const QModelIndex &index)
{
    QModelIndex idx=mMyListModel->index(index.row(),2);
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
void MainWindow::treeAudioClicked(const QModelIndex &index)
{
    if(index.column()==0){
        QModelIndex idx=mMyListModel->index(index.row(),2);

        QString path =idx.data(Qt::UserRole).toString();
        QString title=idx.data().toString();
        if(path.isEmpty() )return;

        QVariantMap map;
        map["Title"]=title;
        map["Url"  ]=path;

        QList<QVariantMap>list;
        list.append(map);
        mPlayer->addToPlaylist(list);

    }else if(index.column()==1){
        QModelIndex idx=mMyListModel->index(index.row(),1);
        if(!idx.isValid())return;

        bool favo   =idx.data(USER_RATED).toBool();
        QString path=mMyListModel->index(index.row(),2).data(Qt::UserRole).toString();
        if (DataBase::setFavorite(path,!favo)){
            mMyListModel->itemFromIndex(index)->setData(!favo,USER_RATED);
            mMyListModel->itemFromIndex(index)->setIcon(favo ? Tumb::icon(I_START):Tumb::icon(I_STARTED));
            qDebug()<<"mainwindow favorit saved";
        }
    }

}

// ------------------------------------------------------
void MainWindow::treeViewContentActivated(const QModelIndex &index)
{

    QModelIndex  pIndex= index.parent();
    QModelIndex  gIndex= pIndex.parent();
    int isfavo   =index.data(USER_FAVORITE).toBool();

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
void MainWindow::listViewActivated(const QModelIndex &index)
{

    QString pId  =mMap.value(MAP_ID);
    QString pName=mMap.value(MAP_TITLE);
    QString gId  =mMap.value(MAP_PID);
    QString gName=mMap.value(MAP_PTITLE);

    int isfavo=index.data(USER_FAVORITE).toBool();

    mMap[MAP_ID]       =index.data(USER_ID).toString();
    mMap[MAP_TITLE]    =index.data(USER_TITLE).toString();
    mMap[MAP_PID]      =pId;
    mMap[MAP_PTITLE]   =pName;
    mMap[MAP_GID]      =gId;
    mMap[MAP_GTITLE]   =gName;
    mMap[MAP_CHILD]    =QString::number(CAT_NULL);
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

    if(chld!=CAT_NULL){
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

        if(id==CAT_ALBUM)
            mMyListModel->chargeAudios(name,CAT_ALBUM);
        else
            mMyListModel->chargeAudios("1",id);

    } else   {
        mMyListModel->chargeAudios(name,id,pName,pId,gName,gId);
    }

    if(isFavo && id!=CAT_ALBUM){
        mTVAudio->setColumnHidden(3,true);
        mTVAudio->setColumnHidden(4,true);
    }else{
        mTVAudio->setColumnHidden(4,id==CAT_ALBUM);
        mTVAudio->setColumnHidden(3,id!=CAT_ALBUM);
    }

    ui->tb_favoritAlbum->setVisible(id==CAT_ALBUM);
    ui->tb_imgAlbum->setVisible(!isFavo);
    setlabelImage();

    mTVAudio->resizeColumnToContents(2);
    mTVAudio->resizeColumnToContents(5);

    ui->stackedWidgetView->setCurrentWidget(ui->pageAudio);

    mTVAudio->setColumnWidth(0,20);
    mTVAudio->setColumnWidth(1,20);

    mTVAudio->header()->setSectionResizeMode(2,QHeaderView::Stretch);
    mTVAudio->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    changeStatusPathText();

    int favo= Setting::albumIsFavorited(name);
    ui->tb_favoritAlbum->setChecked(favo);
    ui->tb_favoritAlbum->setIcon(favo ? Tumb::icon(I_STARTED):Tumb::icon(I_START));

}

// ------------------------------------------------------
void MainWindow::setlabelImage()
{
    int       id=   mMap.value(MAP_ID).toInt();
    QString name=   mMap.value(MAP_TITLE);
    QString path=   mMap.value(MAP_IMG);
    bool  isFavo=   mMap.value(MAP_IS_FAVO).toInt();
    QIcon img;

    if(isFavo && id!=CAT_ALBUM){ img=Tumb::icon(I_FAVO);        }
    else if(id==CAT_ALBUM)     { img=Tumb::iconAlbum(name,path);}
    else                       { img=Tumb::iconnArtist(name);   }

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

        QModelIndex idx=mTVAudio->model()->index(i,2);
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


void MainWindow::on_tb_imgAlbum_clicked()
{
    QString Name=   mMap.value(MAP_TITLE);
    if(Name==tr("Unknown")) return;
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

void MainWindow::changeImageAlbum(int id ,const QString Name)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    D_Picture,
                                                    "Images (*.png *.xpm *.jpg)");

    if(fileName.isEmpty()) return;

    QImage img(fileName);

    img= img.scaled(256,256,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    if(id==CAT_ALBUM)
        img.save(D_ALBUM_CACHE+"/"+Name+".jpg","jpg");
    else if(id==CAT_ARTIST)
        img.save(D_ARTIST_CACHE+"/"+Name+".jpg","jpg");

}

// ------------------------------------------------------
void MainWindow::onFavoritAlbum(int row,bool favo)
{

    QModelIndex index=mMyListModel->index(row,0);
    if(!index.isValid())return;

    bool oldfavo=index.data(USER_RATED).toBool();
    qDebug()<<oldfavo<<favo;
    if(oldfavo==favo)return;

    Setting::setAlbumFavo(index.data(USER_TITLE).toString(),favo);
    mMyListModel->itemFromIndex(index)->setData(favo,USER_RATED);
    qDebug()<<row<<favo;

}

// ------------------------------------------------------
void MainWindow::onAddAlbum(int row)
{
    qDebug()<<row<<"onAddAlbumClicked";
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
//void MainWindow::editCurIndex(const QString &path)
//{
//    qDebug()<<path;
//    QProcess p;
//    p.startDetached("easytag",QStringList()<<path);
//}

// ------------------------------------------------------
void MainWindow::on_actionopen_triggered()
{
    //    if(DataBase::clearDatabase()){
    //        setPathAlbum(ui->comboBoxSwichCat->currentIndex());
    //    }
    //    return;

    DialogOpen *dlg=new DialogOpen(this);
    if(dlg->exec()==QDialog::Accepted){
        bool save=dlg->autoSave(); //TODO :add this to save

        QList<QUrl>list=dlg->listUrls();
        qDebug()<<"MainWindow urls :"<<list;
        if(save)
            mMediaUpdate->addFiles(list);

        mPlayer->addToPlaylist(list);
        mPlayer->playLast();
    }
    delete dlg;
}



void MainWindow::on_tButtonCancelMsg_clicked()
{
    mMediaUpdate->setUpdateDirs(false);
    ui->widgetMessage->setVisible(false);
}

void MainWindow::on_tButtonOkMsg_clicked()
{
    mMediaUpdate->setUpdateDirs(true);
    ui->widgetMessage->setVisible(false);
}
