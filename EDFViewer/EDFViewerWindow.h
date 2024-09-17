/*
The MIT License (MIT)

Copyright (c) 2016 kostasrotas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef EDFVIEWERWINDOW_H
#define EDFVIEWERWINDOW_H

#include <QtWidgets>
#include "edffilehandler.h"
#include "paintwidget.h"
#include "signalswindowhandler.h"
#include "datawindowhandler.h"
#include "annotationswindowhandler.h"

class EDFViewerWindow : public QWidget
{
    Q_OBJECT

public:
    EDFViewerWindow(QWidget *parent = 0);
    ~EDFViewerWindow();
    bool fileIsOpen();
    bool getShowSignal(int);
    void setShowSignal(int, bool);
    double getScreenTime();
    double getStartTime();
    void setStartTime(double);
    double getYZoom();

    char fileOpen(char*filename);
    void exportToCSV (QString filename);

public slots:
    void exportToCSV();
    void fileOpen();
    void changehbar(int);
    void screentimechanged(const QString &);
    void yzoomchanged(const QString &);
    void increaseTime();
    void decreaseTime();

private:

    EDFfilehandler edfh{10000};
    SignalsWindowHandler* signalsWindowHandlerP;
    DataWindowHandler * dataWindowHandlerP;
    AnnotationsWindowHandler *annotationsWindowHandlerP;

    PaintWidget* paintWidget;

    bool fileisopen=0;
    double starttime,screentime=5,yZoom=1;
    bool showsignal[200];

    QScrollBar *hScrollBar;
    QLineEdit* le1,*le2;

};

#endif // EDFVIEWERWINDOW_H
