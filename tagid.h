#ifndef TAGID_H
#define TAGID_H

#include <QObject>

#include <QVariantMap>


//*********************TAG ID**************************
class TagId : public QObject
{
    Q_OBJECT
public:
    explicit TagId(QObject *parent = nullptr);
    static TagId *instance();

    static QVariantMap mediaTags(const QString &file);
    static  void setMediaTags(QVariantMap mMap);
signals:
  void updateFile(const QString &);
public slots:

private:
   //  TagThread *mThread;

};

#endif // TAGID_H
