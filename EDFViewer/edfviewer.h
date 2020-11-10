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

#ifndef EDFVIEWER_H
#define EDFVIEWER_H

#include <QWidget>
#include <QtWidgets>
#include <QApplication>
#include "paintwidget.h"
#include "edffilehandler.h"

#define VERSION "v1.3"

class EDFViewerWindow : public QWidget
{
    Q_OBJECT

    QScrollBar *hScrollBar;
    QLineEdit* le1,*le2;
public:
    EDFViewerWindow(QWidget *parent = 0);
    ~EDFViewerWindow();

    char fileOpen(char*filename);

public slots:
    void fileOpen();
    void changehbar(int);
    void screentimechanged(const QString &);
    void yzoomchanged(const QString &);
    void increaseTime();
    void decreaseTime();
};


void datawindow();
void signalswindow();
void aboutwindow();

extern char fileisopen;
extern EDFfilehandler edfh;
extern PaintWidget* paintWidget;

extern double starttime,screentime,yZoom;
extern char showsignal[];

#endif // EDFVIEWER_H
