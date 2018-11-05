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
#include "player.h"

#include "player_adaptor.h"

#include <QApplication>
#include <QDBusConnection>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //-----------------------------------------------------------------APP INFO
    a.setApplicationName("goldfinch");
    a.setApplicationDisplayName(QObject::tr("Goldfinch"));
    a.setApplicationVersion("0.1");
    a.setOrganizationName("goldfinch");

    //-----------------------------------------------------------------ARGUMENTS
    QStringList args = a.arguments();
    QUrl  pathUrl;

    if(args.count()>1)
    {
        pathUrl=QUrl::fromLocalFile(args.at(1));
        qDebug()<<"Main url:"<<pathUrl;
    }

    //-----------------------------------------------------------------DBUS
    QDBusConnection connection = QDBusConnection::sessionBus();
    // org.mpris.MediaPlayer2.Interface
    if (! QDBusConnection::sessionBus().registerService(QString("org.mpris.MediaPlayer2.%1").arg(a.applicationName())))
    {

        printf ("Unable to register 'org.mpris.MediaPlayer2' service - is another instance of Goldfinch running?\n");

        QDBusInterface dbus("org.mpris.MediaPlayer2.goldfinch",
                            "/org/mpris/MediaPlayer2",
                            "org.mpris.MediaPlayer2");

        if (!dbus.isValid()) { printf ("QDBusInterface is not valid!");return 0; }

        if(!pathUrl.isEmpty()){
            printf ("QDBusInterface is  valid!!!!!");
            dbus.call("SetUrl",pathUrl.toLocalFile());
        }

        return 0;
    }

    //-----------------------------------------------------------------ICONS
    a.setWindowIcon(QIcon::fromTheme("goldfinch",QIcon(":/icons/goldfinch")));

    //-----------------------------------------------------------------TRANSLATIONS

    QDir appDir(a.applicationDirPath());
    appDir.cdUp();
    QString dirPath=  appDir.absolutePath()+"/share/"+a.applicationName();

   QString   locale = QLocale::system().name().section("_",0,0);
    /// اللغة الحالية لجميع البرنامج
    QLocale::setDefault(QLocale(locale));
    /// جلب ترجمات كيوتي
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += locale;
    QTranslator *translatorsys = new QTranslator;
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(translatorsys);
    QString translatorPath=dirPath+"/translations/"+locale+"/"+a.applicationName();
    qDebug()<<"translatorPath"<<translatorPath;
    QTranslator translator;
           translator.load(translatorPath);
           a.installTranslator(&translator);
    QLocale lx=QLocale(locale);
    a.setLayoutDirection(lx.textDirection());
   // a.setLayoutDirection(Qt::RightToLeft);

    //-----------------------------------------------------------------EXEC
    MainWindow w;

    if(!pathUrl.isEmpty()){  w.setUrl(pathUrl.toLocalFile()); }

    w.show();

    return a.exec();
}
