#-------------------------------------------------
#
# Project created by QtCreator 2016-03-05T16:45:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileDir
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp

INCLUDEPATH += "C:/boost_1_60_0/boost_1_60_0"
LIBS += C:/boost_1_60_0/boost_1_60_0/stage/lib/libboost_serialization-vc120-mt-gd-1_60.lib
LIBS += C:/boost_1_60_0/boost_1_60_0/stage/lib/libboost_serialization-vc120-mt-1_60.lib



HEADERS  += mainwindow.h \
    dialog.h


FORMS    += mainwindow.ui \
    dialog.ui
