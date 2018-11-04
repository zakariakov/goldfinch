#-------------------------------------------------
#
# Project created by QtCreator 2017-12-23T17:15:24
#
#-------------------------------------------------

QT       += core gui multimedia \
      multimediawidgets \
      sql dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =goldfinch
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

 target.path = /usr/bin
 INSTALLS +=target
