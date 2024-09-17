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
    annotationswindowhandler.cpp \
    datawindowhandler.cpp \
    edffilehandler.cpp \
        edfviewerwindow.cpp \
    paintwidget.cpp \
    signalswindowhandler.cpp

HEADERS  += edfviewer.h \
    EDFViewerWindow.h \
    annotationswindowhandler.h \
    datawindowhandler.h \
    paintwidget.h \
    edffilehandler.h \
    signalswindowhandler.h

LIBS += -L$$PWD/

RESOURCES +=
