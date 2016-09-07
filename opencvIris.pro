#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T17:28:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvIris
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp

HEADERS  += mainwindow.h \
    player.h

FORMS    += mainwindow.ui

INCLUDEPATH += "/usr/local/include/opencv2"
LIBS += `pkg-config --cflags --libs opencv`

RESOURCES += \
    resources.qrc
