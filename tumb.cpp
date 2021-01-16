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

#include "tumb.h"
#include <QImageReader>
#include <QDir>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QBitmap>
#include <QPalette>

Q_GLOBAL_STATIC(Tumb, TumbInstance)
Tumb *Tumb::instance()
{
    return TumbInstance();
}

Tumb::Tumb()
{
    QDir dir;
    dir.mkpath(CACHE_ALBUM);
    dir.mkpath(CACHE_ARTIST);
    dir.mkpath(TEMP_CACH);
    mapIcons=new QMap<QString,QString>;
}
QIcon  Tumb::iconAlbum(const QString &title, const QString &dirPath)
{
    QString iconPath;
    if(instance()->mapIcons->contains(title)){
        //qDebug()<<"instance()exist"<<title;
        iconPath=  instance()->mapIcons->value(title);
        if(!QFile::exists(iconPath))
            iconPath=imageAlbumPath(title,dirPath);

    }else{

        iconPath=imageAlbumPath(title,dirPath);

    }
    int size=instance()->m_size;
    /*
   QPixmap pix=QPixmap(iconPath);
//rQPixmap pix(iconPath);
//if(pix.width()>pix.height())
//     pix=pix.scaledToWidth(size,Qt::SmoothTransformation);
//else
//  pix=pix.scaledToHeight(size,Qt::SmoothTransformation);
// pix=pix.scaledToWidth(size,Qt::SmoothTransformation);

//    qDebug()<<"tamb pix"<<pix.width()<<pix.height();
   int x=(size-pix.width())/2;
   int y=(size-pix.height())/2;

    QPixmap pixRet(size,size);
    pixRet.fill(Qt::transparent);
    QPainter painter(&pixRet);
    painter.drawPixmap(0,0,size,size,pix);
QPixmap pix=QPixmap(iconPath).scaled(size,size,Qt::KeepAspectRatio,Qt::SmoothTransformation);
//QPixmap pix=QPixmap(iconPath).scaledToWidth(96,Qt::SmoothTransformation);
    QIcon icon=QIcon(pix);

    return  icon;
    */
    QPixmap pix=QPixmap(iconPath).scaled(size,size,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    return   QIcon(pix);
}

QString Tumb::imageAlbumPath(const QString &title,const QString &dirPath)
{
  // qDebug()<<"Art"<<title<<dirPath;
    // Imge from user in cache
    // QString title=index.data().toString()+".jpg";
    if(QFile::exists(CACHE_ALBUM+"/"+title+".jpg"))
        return CACHE_ALBUM+"/"+title+".jpg";

    // Image from defined name
    //QString dirPath=index.data(D_IMGPATH).toString();
    QDir dir(dirPath);

    QStringList listImg=QStringList()<<"cover.jpg"<<"cover.png"
                                    <<".cover.jpg"<<".cover.png"
                                   <<".AlbumArt.png" <<".AlbumArt.jpg"
                                  <<"AlbumArtSmall.jpg"<<"AlbumArtSmall.png"
                                 <<".AlbumArtSmall.jpg"<<".AlbumArtSmall.png"
                                <<"AlbumArtLarge.jpg"<<"AlbumArtLarge.png"
                               <<".AlbumArtLarge.jpg"<<".AlbumArtLarge.png"
                              <<"folder.jpg"<<"Folder.jpg"
                             <<"folder.png"<<"Folder.png";


    foreach (QString s, listImg) {
        if(QFile::exists(dir.filePath(s))){
            instance()->mapIcons->insert(title,dir.filePath(s));
            return  dir.filePath(s);
        }
    }

    // Image any image from path
    QList<QByteArray> list=QImageReader::supportedImageFormats();
    QStringList filterList;
    foreach (QString s, list) {filterList<<"*."+s.toLatin1(); }

    QStringList imglist=dir.entryList(filterList,QDir::Files|QDir::Hidden);
    foreach (QString s, imglist) {
        QImageReader reader(dir.filePath(s));
        int min=qMin(reader.size().width(),reader.size().height());
        if(reader.canRead() && min>48){

            int max=qMax(reader.size().width(),reader.size().height());
            if(max>256){
                QImage image(dir.filePath(s));
                image= image.scaled(256,256,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                if(image.save(dirPath+"/cover.jpg")){
                    instance()->mapIcons->insert(title,dir.filePath(s));
                    return dirPath+"/cover.jpg";
                }
            }
            instance()->mapIcons->insert(title,dir.filePath(s));
            return dir.filePath(s);
        }

    }

    // Image from source
    return ":/icons/cover";

}

QIcon Tumb::iconnArtist(const QString &title)
{
    QString iconPath=imageArtiPath(title);
    QPixmap pix(iconPath);
   // if(iconPath!=":/icons/cover"){
        QBitmap bitmap(":/icons/mask");
        // pix=  pix.scaled(256,256);
        pix.setMask(bitmap);
   // }


    return QIcon(pix);
}
QString Tumb::imageArtiPath(const QString &title)
{
    // Imge from user in cache
    //QString title=index.data().toString()+".jpg";
    if(QFile::exists(CACHE_ARTIST+"/"+title+".jpg"))
        return CACHE_ARTIST+"/"+title+".jpg";
    // Image from source
    return ":/icons/artist";
}

QIcon Tumb::iconColorized(QString file,QColor color)
{

    // QPixmap pix=(QPixmap(file));

    QImage sourceImage; sourceImage.load(file);
    QImage resultImage(sourceImage);
    // QImage destinationImage(sourceImage);
    //     QImage destinationImage;
    //     destinationImage.fill(color);

    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Overlay);
    // painter.drawImage(0, 0, destinationImage);
    painter.fillRect(sourceImage.rect(),color);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawImage(0, 0, sourceImage);

    return QIcon(QPixmap::fromImage(resultImage ));
}

QIcon Tumb::icon(int name)
{
  bool isLeft= QApplication::isLeftToRight();
 QColor color= QApplication::palette().text().color();


    switch (name) {
    case I_ARTIST:     return iconColorized(":/icons/artist-16",color);
    case I_Album:      return iconColorized(":/icons/cover-16",color);
    case I_Genre:      return iconColorized(":/icons/genre-16",color);
    case I_PAUSE:      return iconColorized(":/icons/pause",color);
   case I_STOP:      return iconColorized(":/icons/stop",color);
    case I_ADD:        return iconColorized(":/icons/add",color);
    case I_ADD_ALBUM:  return iconColorized(":/icons/album-add",color);
    case I_START:      return iconColorized(":/icons/star",color);
    case I_STARTED:    return iconColorized(":/icons/stared",QApplication::palette().highlight().color());
    case I_VOLUME:     return iconColorized(":/icons/volume",color);
    case I_VOLUME_OF:  return QIcon(":/icons/volume-off");
    case I_IMG:        return iconColorized(":/icons/preview",color);
    case I_LIST_REMOVE:return iconColorized(":/icons/list-remove",color);
    case I_CLEAN:      return iconColorized(":/icons/cleanup",color);
    case I_M_SHUFLE:   return iconColorized(":/icons/media-shuffle",color);
    case I_M_SEQUEN:   return iconColorized(":/icons/media-sequential",color);
    case I_M_REPEAT:   return iconColorized(":/icons/media-repeat",color);
    case I_M_REPEAT_ONE:return iconColorized(":/icons/media-repeat-one",color);
    case I_M_PLAY_ONE: return iconColorized(":/icons/media-paly-one",color);
    case I_AROW_UP:       return iconColorized(":/icons/pan-up",color);
    case I_AROW_DOWN:   return iconColorized(":/icons/pan-down",color);
    case I_FAVO:       return iconColorized(":/icons/favo",color);
    case I_MENU:       return iconColorized(":/icons/menu",color);
    case I_PROPERTIES: return iconColorized(":/icons/properties",color);
    case I_TOGGLE:  return iconColorized(":/icons/toggle",color);

    case I_OPTIONS: return iconColorized(":/icons/properties",QApplication::palette().highlight().color());
    case I_QUIT: return iconColorized(":/icons/window-close",color);
    case I_Close: return iconColorized(":/icons/window-close",QApplication::palette().highlight().color());
    case I_FIND: return iconColorized(":/icons/find-16",color);
    case I_ALBUM_SMALL: return iconColorized(":/icons/cover-album",color);
    case I_PANE_HIDE:
        if(isLeft)     return iconColorized(":/icons/pane-hide",color);
        else           return iconColorized(":/icons/pane-show",color);
    case I_PANE_SHOW:
        if(isLeft)     return iconColorized(":/icons/pane-show",color);
        else           return iconColorized(":/icons/pane-hide",color);
    case I_PLAY:
        if(isLeft)     return iconColorized(":/icons/play",color);
        else           return iconColorized(":/icons/play-rtl",color);
    case I_NEXT:
        if(isLeft)     return iconColorized(":/icons/next",color);
        else           return iconColorized(":/icons/prev",color);
    case I_PREV:
        if(isLeft)     return iconColorized(":/icons/prev",color);
        else           return iconColorized(":/icons/next",color);
    case I_CLEAR:
        if(isLeft)     return iconColorized(":/icons/edit-clear",QApplication::palette().highlight().color());
        else           return iconColorized(":/icons/edit-clear-rtl",QApplication::palette().highlight().color());

    default:  break;
    }
    return QIcon();
}
