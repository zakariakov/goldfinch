#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QSettings>
class Setting : public QSettings
{
Q_OBJECT
public:
   explicit Setting(QObject *parent = nullptr);
    void setAlbumFavo(const QString &title,bool favo);
    bool albumIsFavorited(const QString &title);
   QString albumImgPath(const QString &title){return value(title).toString();}
   QStringList favoretedAlbum();

};

#endif // SETTING_H
