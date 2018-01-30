#-------------------------------------------------
#
# Project created by QtCreator 2017-05-31T17:06:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommunityDetectionGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../Communities.cpp \
    ../../Graph.cpp \
    comm.cpp \
    ../../ylog.cpp

HEADERS  += mainwindow.h \
    comm.h \
    ../../ylog.h \
    ../../Communities.h

FORMS    += mainwindow.ui \
    comm.ui
