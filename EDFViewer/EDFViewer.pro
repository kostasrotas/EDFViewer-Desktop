#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T15:57:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EDFViewer
TEMPLATE = app

RC_ICONS = edfviewer_ic.ico

SOURCES += main.cpp\
    edffilehandler.cpp \
        edfviewerwindow.cpp \
    paintwidget.cpp \
    datawindows.cpp

HEADERS  += edfviewer.h \
    paintwidget.h \
    edffilehandler.h

LIBS += -L$$PWD/

RESOURCES +=
