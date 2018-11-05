#/***************************************************************************
# *      Project created by QtCreator 2018-06-01T17:15:24                   *
# *                                                                         *
# *    goldfinch Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>         *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 3 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************/


QT       += core gui multimedia \
      multimediawidgets \
      sql dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =goldfinch
DESTDIR = usr/bin
TEMPLATE = app

CONFIG += qt \
          release
CONFIG += thread
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mainwindow.cpp \
    database.cpp \
    mylistmodel.cpp \
    treeviewcontent.cpp \
    listview.cpp \
    listitemdelegate.cpp \
    tumb.cpp \
    mediaupdate.cpp \
    playlistmodel.cpp \
    playercontrols.cpp \
    player.cpp \
    listeditor.cpp \
    widgetplaylist.cpp \
    mycontentmodel.cpp \
    treeitemdelegate.cpp \
    setting.cpp \
    treeviewaudio.cpp \
    widgetimageinfo.cpp \
    dialogopen.cpp \
    player_adaptor.cpp \
    dialogoptions.cpp \
    slider.cpp \
    propertiesfile.cpp \
    searchbar.cpp

HEADERS += \
        mainwindow.h \
    database.h \
    mylistmodel.h \
    defines.h \
    treeviewcontent.h \
    listview.h \
    listitemdelegate.h \
    tumb.h \
    mediaupdate.h \
    playlistmodel.h \
    playercontrols.h \
    player.h \
    listeditor.h \
    widgetplaylist.h \
    mycontentmodel.h \
    treeitemdelegate.h \
    setting.h \
    treeviewaudio.h \
    widgetimageinfo.h \
    dialogopen.h \
    player_adaptor.h \
    dialogoptions.h \
    slider.h \
    propertiesfile.h \
    searchbar.h

FORMS += \
        mainwindow.ui \
    dialogopen.ui \
    dialogoptions.ui \
    propertiesfile.ui

RESOURCES += \
    icons.qrc

TRANSLATIONS    =usr/share/goldfinch/translations/ar/goldfinch.ts\
                 usr/share/goldfinch/translations/fr/goldfinch.ts\
                 usr/share/goldfinch/translations/en/goldfinch.ts

#------------ INSTTALL ---------------
desktop.file=goldfinch.desktop
desktop.path  =/usr/share/applications
icons.file  =icons/icons/hicolor/*
icons.path  =/usr/share/icons/hicolor
target.path =/usr/bin
 INSTALLS +=target icons desktop
