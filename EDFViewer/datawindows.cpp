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
QCheckBox *signalcb[200];
QWidget *signalwp;

void closesignalswindow(){
    int i;
    for (i=0;i<edfh.numofsignals;i++)
        showsignal[i]=signalcb[i]->isChecked()?1:0;
    signalwp->close();
    paintWidget->repaint();
}

void seldeselall(){
    int i;
    Qt::CheckState newvalue=Qt::Checked;
    for (i=0;i<edfh.numofsignals;i++)
        if (signalcb[i]->isChecked()){
            newvalue=Qt::Unchecked;
            break;
        }
    for (i=0;i<edfh.numofsignals;i++){
        signalcb[i]->setCheckState(newvalue);
    }
}

void signalswindow(){
    char str[256];
    float phmin,phmax;
    int spr,digmin,digmax;\
    char name[256],dim[256];
    int i;
    int numOfColums=1;
    if (fileisopen==0)
        return;
    signalwp=new QWidget();
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb2,*hb1,*hb3;
    signalwp->setAttribute(Qt::WA_DeleteOnClose);
    signalwp->setWindowModality(Qt::ApplicationModal);
    signalwp->setWindowTitle("EDF Signals");
    signalwp->setLayout(vb);
    numOfColums=1+edfh.numofsignals/8;
    for (i=0;i<edfh.numofsignals;i++){
        if (i%numOfColums==0){
            hb1=new QHBoxLayout;
            vb->addLayout(hb1);
        }
        hb2=new QHBoxLayout;
        signalcb[i]=new QCheckBox();
        signalcb[i]->setCheckState(showsignal[i]!=0?Qt::Checked:Qt::Unchecked);
        hb2->addWidget(signalcb[i]);
        edfh.getsignaldata(i,name,NULL,dim,&phmin,&phmax,&digmin,&digmax,NULL,&spr);
        sprintf(str,"Signal %d [%s]\nPhysical %.2f...%.2f%s\nDigital :%d...%d\nSamples per record :%d\n",i+1,name,phmin,phmax,dim,digmin,digmax,spr);
        hb2->addWidget(new QLabel(str));
        hb2->setStretch(hb2->count()-1,1);
        hb1->addLayout(hb2);
    }
    hb3=new QHBoxLayout;
    button = new QPushButton("Deselect/Select All");
    QObject::connect(button,&QPushButton::clicked,seldeselall);
    hb3->addWidget(button);
    button = new QPushButton("O.K.");
    QObject::connect(button,&QPushButton::clicked,closesignalswindow);
    hb3->addWidget(button);
    vb->addLayout(hb3);
    signalwp->show();
}


void datawindow(){
    char str[1024];
    if (fileisopen==0)
        return;
    sprintf(str,"Patient id:%s\nRecord id:%s\nStarttime:%s\nStartdate:%s\nRecord duration:%.2f\nNum of Records:%d\nTotal time:%.2f\nNum of signals:%d",edfh.patientid,edfh.recordid,edfh.starttimerec,edfh.startdaterec,edfh.recduration,edfh.numofdatarecords,edfh.totaltime(),edfh.numofsignals);
    QWidget *wp=new QWidget();
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    wp->setAttribute(Qt::WA_DeleteOnClose);
    wp->setWindowModality(Qt::ApplicationModal);
    wp->setWindowTitle("EDF DATA");
    wp->setLayout(vb);
    vb->addWidget(new QLabel(str));
    wp->show();
    button = new QPushButton("O.K.");
    vb->addWidget(button);
    QObject::connect(button,SIGNAL(clicked()),wp,SLOT(close()));
}

void aboutwindow(){
    char str[256];
    QWidget *wp=new QWidget();
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    wp->setAttribute(Qt::WA_DeleteOnClose);
    wp->setWindowModality(Qt::ApplicationModal);
    wp->setWindowTitle("About EDF-Viewer");
    wp->setLayout(vb);
    sprintf(str,"          EDF Viewer %s",VERSION);
    vb->addWidget(new QLabel(str));
    vb->addWidget(new QLabel("\n\n  Program used to display EDF files.\n  Released under MIT License (MIT)\n  Created with QT Creator\n  See Licences.txt for Licences\n\n   (c)2016 Rotas Kostas, email:kostasrotas@yahoo.com  "));
    wp->show();
    button = new QPushButton("O.K.");
    vb->addWidget(button);
    QObject::connect(button,SIGNAL(clicked()),wp,SLOT(close()));
}
