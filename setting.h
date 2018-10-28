#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QSettings>
class Setting : public QSettings
{
    Q_OBJECT
public:
    explicit            Setting(QObject *parent = nullptr);
    static void         setAlbumFavo(const QString &title,bool favo);
    static bool         albumIsFavorited(const QString &title);
    static QString      albumImgPath(const QString &title);
    static QStringList  favoretedAlbum();

};

#endif // SETTING_H
