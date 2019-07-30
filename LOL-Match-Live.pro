#-------------------------------------------------
#
# Project created by QtCreator 2019-07-10T19:45:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LOL-Match-Live
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    lolmatchliveapp.cpp \
    components/mainwindow/mainwindow.cpp \
    components/mainwindow/titlebar.cpp \
    components/mainwindow/contentarea.cpp \
    features/fontloader.cpp \
    features/colorboard.cpp \
    components/mainwindow/live/livearea.cpp \
    components/mainwindow/setting/settingarea.cpp \
    components/mainwindow/areatitle.cpp \
    components/mainwindow/live/matchinfo.cpp \
    components/mainwindow/live/pointinfoarea.cpp \
    components/mainwindow/live/pointinfo.cpp \
    components/mainwindow/setting/settingtitle.cpp \
    components/mainwindow/setting/radiobuttongroup.cpp \
    features/setting/settinghandler.cpp \
    features/live/matchinfoparser.cpp \
    components/mainwindow/setting/matchidinput.cpp \
    components/popupwindow/popupwindow.cpp \
    components/popupwindow/popuppointinfo.cpp

HEADERS  += \
    lolmatchliveapp.h \
    components/mainwindow/mainwindow.h \
    components/mainwindow/titlebar.h \
    components/mainwindow/contentarea.h \
    features/fontloader.h \
    features/colorboard.h \
    components/mainwindow/live/livearea.h \
    components/mainwindow/setting/settingarea.h \
    components/mainwindow/areatitle.h \
    components/mainwindow/live/matchinfo.h \
    components/mainwindow/live/pointinfoarea.h \
    components/mainwindow/live/pointinfo.h \
    components/mainwindow/setting/settingtitle.h \
    components/mainwindow/setting/radiobuttongroup.h \
    features/setting/settinghandler.h \
    features/live/matchinfoparser.h \
    components/mainwindow/setting/matchidinput.h \
    components/popupwindow/popupwindow.h \
    components/popupwindow/popuppointinfo.h

RESOURCES += \
    assets.qrc
