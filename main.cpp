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

void help()
{
    printf("Usage: goldfinch [OPTION]\n");
    printf("Usage: goldfinch <url>\n");
    puts("goldfinch v: 0.1 \n" );
    puts("OPTION:\n");
    puts(" -h --help        Print this help.");
    puts(" -play        Play curent song");
    puts(" -pause       Pause curent song");
    puts(" -pp          play/pause curent song");
    puts(" -next        Next song");
    puts(" -prev        previous song");
    puts(" -hide        Hide window");
    puts(" -showhiden   Show window hiden");
    puts(" -togglehide  Toggle show/hide window");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //-----------------------------------------------------------------APP INFO
    a.setApplicationName(APP_NAME);
    a.setApplicationDisplayName(QObject::tr("Goldfinch"));
    a.setApplicationVersion(APP_VERTION);
    a.setOrganizationName(APP_NAME);

    //-----------------------------------------------------------------ARGUMENTS
    QStringList args = a.arguments();
    QUrl  pathUrl;
    bool play=false;
    bool pause=false;
    bool playpause=false;
    bool next=false;
    bool previous=false;
    bool hide=false;
   // bool raise=false;
    bool showhiden=false;
    bool toggleHide=false;
    if(args.count()>1)
    {
        if(args.at(1)=="-h"||args.at(1)=="--help"){
            help();
            return 0;
        }
        else if(args.at(1)=="-play")     play=true;
        else if(args.at(1)=="-pause")    pause=true;
        else if(args.at(1)=="-pp")playpause=true;
        else if(args.at(1)=="-next")     next=true;
        else if(args.at(1)=="-prev") previous=true;
        else if(args.at(1)=="-hide")     hide=true;
       /// else if(args.at(1)=="-raise")    raise=true;
        else if(args.at(1)=="-showhiden")    showhiden=true;
        else if(args.at(1)=="-togglehide")    toggleHide=true;
        else{
            //            QString str;
            //            for (int i = 1; i < args.count(); ++i) {
            //                str+=args.at(i)+" ";
            //            }
            pathUrl=QUrl::fromLocalFile(args.at(1));
            qDebug()<<"Main url:"<<pathUrl;
        }

    }

    //-----------------------------------------------------------------DBUS
    QDBusConnection connection = QDBusConnection::sessionBus();
    // org.mpris.MediaPlayer2.Interface
    if (! QDBusConnection::sessionBus().registerService(QString("org.mpris.MediaPlayer2.%1").arg(a.applicationName())))
    {

        printf ("Unable to register 'org.mpris.MediaPlayer2' service - is another instance of Goldfinch running?\n");

        QDBusInterface dbus("org.mpris.MediaPlayer2.goldfinch",
                            "/org/mpris/MediaPlayer2",
                            "org.mpris.MediaPlayer2.Player");

        if (!dbus.isValid()) { printf ("QDBusInterface is not valid!");return 0; }

        if(play)     { dbus.call("Play");return 0;}
        if(pause)    { dbus.call("Pause");return 0;}
        if(playpause){ dbus.call("PlayPause");return 0;}
        if(next)     { dbus.call("Next");return 0;}
        if(previous) { dbus.call("Previous");return 0;}



        if(!pathUrl.isEmpty()){
            printf ("QDBusInterface is  valid!!!!!");
            dbus.call("SetUrl",pathUrl.toLocalFile());
        }
        QDBusInterface dbus2("org.mpris.MediaPlayer2.goldfinch",
                             "/org/mpris/MediaPlayer2",
                             "org.mpris.MediaPlayer2");
        if (!dbus2.isValid()) { printf ("QDBusInterface 2 is not valid!");return 0; }
        if(toggleHide) { dbus2.call("ToggleHide");return 0;}

        if(hide) dbus2.call("Hide");
        else     dbus2.call("Raise");

        return 0;
    }

    //-----------------------------------------------------------------ICONS
    //a.setWindowIcon(QIcon::fromTheme("audio-player",QIcon(":/icons/goldfinch")));
    a.setWindowIcon(QIcon::fromTheme("audio-player",QIcon::fromTheme("goldfinch",QIcon(":/icons/goldfinch"))));

    //-----------------------------------------------------------------TRANSLATIONS

    QDir appDir(a.applicationDirPath());
    appDir.cdUp();
    QString dirPath=  appDir.absolutePath()+"/share/"+a.applicationName();

    QString   localeSymbol = QLocale::system().name().section("_",0,0);
    QSettings settings;
    settings.beginGroup("Window");
    QString userLocal=settings.value("Language").toString();
    settings.endGroup();

    if(!userLocal.isEmpty())    localeSymbol=userLocal;


    /// اللغة الحالية لجميع البرنامج
    QLocale::setDefault(QLocale(localeSymbol));
    /// جلب ترجمات كيوتي
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += localeSymbol;
    QTranslator *translatorsys = new QTranslator;
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(translatorsys);
    /// جلب ترجمة البرنامج



    QString translatorPath=dirPath+"/translations/"+localeSymbol+"/"+a.applicationName();
    QTranslator translator;
    translator.load(translatorPath);
    a.installTranslator(&translator);
    QLocale lx=QLocale(localeSymbol);
   a.setLayoutDirection(lx.textDirection());
 //       a.setLayoutDirection(Qt::LeftToRight);

    //-----------------------------------------------------------------EXEC
    MainWindow w;
    //     new PlayerAdaptor(w.player());
    //          new MainAdaptor(w.player());
    //         connection.registerObject(QString("/org/mpris/MediaPlayer2"),QString("Player"), w.player());
    if(!pathUrl.isEmpty()){
        w.setUrl(pathUrl.toLocalFile());
        qDebug()<<"Main url2:"<<pathUrl;
    }

    if(showhiden)
        w.hide();
    else
        w.show();

    return a.exec();
}
