#ifndef SIGNALSWINDOWHANDLER_H
#define SIGNALSWINDOWHANDLER_H

#include <QtWidgets>
#include "edffilehandler.h"

class EDFViewerWindow;

class SignalsWindowHandler:QObject
{
Q_OBJECT

public:
    SignalsWindowHandler(EDFfilehandler&,EDFViewerWindow&);
    void open();

private:
    QCheckBox *signalcb[200];
    QWidget *signalwp;

    EDFfilehandler &edfh;
    EDFViewerWindow &edfViewerWindow;

private slots:
    void seldeselall();
    void closesignalswindow();

};

#endif // SIGNALSWINDOWHANDLER_H
