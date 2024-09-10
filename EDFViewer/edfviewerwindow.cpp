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

#include "edfviewer.h"

PaintWidget* paintWidget;
char fileisopen=0;
EDFfilehandler edfh(10000);
double starttime,screentime=5,yZoom=1;
char showsignal[200];



EDFViewerWindow::EDFViewerWindow(QWidget *parent)
    : QWidget(parent){
    QVBoxLayout *vb;
    QHBoxLayout *hb;
    QMenu *menu;
    QAction *act;
    QMenuBar* menuBar;
    QPushButton* btn;

    vb=new QVBoxLayout();
    hb=new QHBoxLayout();
    vb->addLayout(hb);
    menuBar = new QMenuBar();
    menu= menuBar->addMenu(tr("&File"));
    act=new QAction(tr("&Open"), this);
    menu->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(fileOpen()));
    act=new QAction(tr("&Exit"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    menu->addAction(act);
    menu= menuBar->addMenu(tr("&EDF"));
    act=new QAction(tr("&EDF Data"), this);
    menu->addAction(act);
    QObject::connect(act,&QAction::triggered,datawindow);
    act=new QAction(tr("&Signals"), this);
    menu->addAction(act);
    QObject::connect(act,&QAction::triggered,signalswindow);
    hb->addWidget(menuBar);
    hb->setStretch(hb->count()-1,1);
    menuBar = new QMenuBar();
    menu= menuBar->addMenu(tr("&Help"));
    act=new QAction(tr("&About"), this);
    menu->addAction(act);
    QObject::connect(act,&QAction::triggered,aboutwindow);
    hb->addWidget(menuBar);

    vb->addWidget(paintWidget=new PaintWidget());
    vb->setStretch(vb->count()-1,1);
    hScrollBar = new QScrollBar(Qt::Horizontal, this);
    connect(hScrollBar, SIGNAL(valueChanged(int)),this,SLOT(changehbar(int)));
    hb=new QHBoxLayout();
    hb->addWidget(hScrollBar);
    hb->setStretch(hb->count()-1,1);
    btn=new QPushButton("-");
    btn->setFixedSize(btn->fontMetrics().boundingRect("+").width()*2,btn->fontMetrics().boundingRect("+").height()*1.5);
    connect(btn,SIGNAL(pressed()),this, SLOT(decreaseTime()));
    hb->addWidget(btn);
    le1=new QLineEdit();
    le1->setText(QString::number(screentime));
    le1->setFixedSize(le1->fontMetrics().boundingRect("0").width()*3,le1->fontMetrics().boundingRect(le1->text()).height()*1.5);
    connect(le1, SIGNAL(textChanged(const QString &)), this, SLOT(screentimechanged(const QString &)));
    hb->addWidget(le1);
    hb->addWidget(new QLabel("sec"));
    btn=new QPushButton("+");
    btn->setFixedSize(btn->fontMetrics().boundingRect("+").width()*2,btn->fontMetrics().boundingRect("+").height()*1.5);
    connect(btn,SIGNAL(pressed()),this, SLOT(increaseTime()));
    hb->addWidget(btn);
    hb->addWidget(new QLabel("YZoom:"));
    le2=new QLineEdit();
    le2->setText(QString::number(yZoom));
    le2->setFixedSize(le2->fontMetrics().boundingRect("0").width()*4,le2->fontMetrics().boundingRect(le2->text()).height()*1.5);
    connect(le2, SIGNAL(textChanged(const QString &)), this, SLOT(yzoomchanged(const QString &)));
    hb->addWidget(le2);


    vb->addLayout(hb);
    resize(450,400);
    setLayout(vb);
    setWindowTitle("EDFViewer");
}


void EDFViewerWindow::increaseTime(){
    le1->setText(QString::number(atol(le1->text().toStdString().c_str())+1));
}

void EDFViewerWindow::decreaseTime(){
    if (screentime>1)
        le1->setText(QString::number(atol(le1->text().toStdString().c_str())-1));
}


void EDFViewerWindow::changehbar(int v){
    if (fileisopen==0)
        return;
   starttime=(v*(edfh.totaltime()-screentime))/(hScrollBar->maximum()-hScrollBar->minimum());
   repaint();
}


void EDFViewerWindow::screentimechanged(const QString & t){
    if (atol(t.toStdString().c_str())>0)
        screentime=atol(t.toStdString().c_str());
    repaint();
}

void EDFViewerWindow::yzoomchanged(const QString & t){
    if (atof(t.toStdString().c_str())>0)
        yZoom=atof(t.toStdString().c_str());
    repaint();
}

char EDFViewerWindow::fileOpen(char*pathname){
    char str[256];
    char *str1;
    int i;
    if (edfh.openfile(pathname)==0){
        fileisopen=0;
        QMessageBox::information(this, "Cannot load file...",pathname);
        return 0;
    }
    str1=strrchr(pathname,'//');
    if (str1==NULL)
        str1=pathname;
    sprintf(str,"EDFViewer : %s",(str1+1));
    setWindowTitle(str);
    fileisopen=1;
    starttime=0;
    for (i=0;i<edfh.numofsignals;i++)
        showsignal[i]=1;
    repaint();
    return 1;
}

void EDFViewerWindow::fileOpen(){
    char pathname[256];
    QFileDialog dialog(this);
    strcpy(pathname ,dialog.getOpenFileName(this,"Open File",".", "EDF files (*.edf)").toUtf8());
    fileOpen(pathname);
}

EDFViewerWindow::~EDFViewerWindow()
{

}
