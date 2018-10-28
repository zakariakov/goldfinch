#ifndef TUMB_H
#define TUMB_H

#include "defines.h"
#include <QObject>
#include <QIcon>
#include <QMap>

#define I_ARTIST 1
#define I_Album  2
#define I_Genre  3
#define I_PLAY   4
#define I_PAUSE  5
#define I_ADD    6
#define I_ADD_ALBUM    7
#define I_PANE_HIDE    8
#define I_PANE_SHOW    9
#define I_NEXT   10
#define I_PREV   11
#define I_IMG    12
#define I_START    13
#define I_STARTED   14
#define I_VOLUME   15
#define I_VOLUME_OF    16
#define I_LIST_REMOVE    17
#define I_CLEAN    18
#define I_M_SHUFLE    19
#define I_M_SEQUEN    20
#define I_M_REPEAT    21
#define I_M_REPEAT_ONE    22
#define I_M_PLAY_ONE    23
#define I_AROW    24
#define I_AROW_RTL    25
#define I_FAVO    26
#define I_MENU    27
#define I_PROPERTIES    28

class Tumb
{
public:
    Tumb();
    //!
    static Tumb *instance();
     static QIcon iconAlbum(const QString &title, const QString &dirPath);
     static QString imageAlbumPath(const QString &title, const QString &dirPath);
     static QIcon iconnArtist(const QString &title);
     static QString imageArtiPath(const QString &title);
     static QIcon iconColorized(QString file, QColor color);
     static QIcon icon(int name);

public slots:
   static void setSize(int size){instance()->m_size=size;}
//   static void setIconColor(QColor col,QColor coled)
//   {instance()->m_color=col;instance()->m_colorEd=coled;}
private:
     QMap<QString,QString>*mapIcons;
int m_size;
//QColor m_color;
//QColor m_colorEd;
};

#endif // TUMB_H
