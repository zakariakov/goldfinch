#include "mainwindow.h"
#include "player.h"

#include "player_adaptor.h"

#include <QApplication>
#include <QDBusConnection>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //-----------------------------
    a.setApplicationName("goldfinch");
     a.setApplicationDisplayName(QObject::tr("Goldfinch"));
    a.setApplicationVersion("0.1");
    a.setOrganizationName("goldfinch");
    //-----------------------------
    QDBusConnection connection = QDBusConnection::sessionBus();
  // org.mpris.MediaPlayer2.Interface
    if (! QDBusConnection::sessionBus().registerService(QString("org.mpris.MediaPlayer2.%1").arg(a.applicationName())))
    {
        printf ("Unable to register 'org.mpris.MediaPlayer2' service - is another instance of Goldfinch running?\n");
        return 1;
    }

    //-----------------------------
    a.setWindowIcon(QIcon::fromTheme("goldfinch",QIcon(":/icons/goldfinch")));
a.setLayoutDirection(Qt::RightToLeft);
    MainWindow w;
    new player_adaptor(w.player());

    connection.registerObject(QString("/org/mpris/MediaPlayer2").arg(a.applicationName()), w.player());

    w.show();

    return a.exec();
}
