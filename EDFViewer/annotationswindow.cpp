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
#include<qdebug.h>

QWidget *annotationswp;

void annotationswindow(){
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb2,*hb1,*hb3;
    QListWidget *listwp;
    static char annotStr[1024];
    static char timeStr[1024];
    char str[256];
    long i,j;

    if (fileisopen==0)
        return;
    annotationswp=new QWidget();
    annotationswp->setAttribute(Qt::WA_DeleteOnClose);
    annotationswp->setWindowModality(Qt::ApplicationModal);
    annotationswp->setWindowTitle("EDF Annotations");
    annotationswp->setLayout(vb);

    if (edfh.annotsignal==-1){
        vb->addWidget(new QLabel("This file does not contain annotations"));
    }
    else{
        listwp=new QListWidget;
        for (i=0;i<edfh.numofdatarecords;i++){
            edfh.getannotation(i,1,annotStr,timeStr);
            if (annotStr[0]!=0){
                sprintf(str,"[%s] %s",timeStr,annotStr);
                listwp->addItem(str);
                //qDebug() << "annotation ["<<timeStr<<"] "<<annotStr<<" "<<atime;
            }
        }
        vb->addWidget(listwp);
        QObject :: connect (listwp,&QListWidget::currentRowChanged,[&]{
            char str[256];
            qDebug()<<"Move to "<<listwp->currentRow();
            //edfh.getannotation(listwp->currentRow(),1,annotStr,str);
            //qDebug()<<"time "<<str;
        });
    }




    hb3=new QHBoxLayout;
    button = new QPushButton("O.K.");
    QObject::connect(button,SIGNAL(clicked()),annotationswp,SLOT(close()));
    hb3->addWidget(button);
    vb->addLayout(hb3);
    annotationswp->show();

/*    char str[256];
    char str2[256];
    char useTips;
    float phmin,phmax;
    int spr,digmin,digmax;
    char name[256],dim[256];
    int i,j;
    int numOfColums=1;
    if (fileisopen==0)
        return;
    signalwp=new QWidget();
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb2,*hb1,*hb3;
    QVBoxLayout *vbn[10];
    signalwp->setAttribute(Qt::WA_DeleteOnClose);
    signalwp->setWindowModality(Qt::ApplicationModal);
    signalwp->setWindowTitle("EDF Signals");
    signalwp->setLayout(vb);
    if (edfh.numofgraphsingals()<=24){
        useTips=0;
        numOfColums=1+edfh.numofgraphsingals()/8;
    }
    else{
        useTips=1;
        numOfColums=1+edfh.numofgraphsingals()/16;
    }
    hb1=new QHBoxLayout;
    vb->addLayout(hb1);
    for (i=0;i<numOfColums;i++){
        vbn[i]=new QVBoxLayout;
        hb1->addLayout(vbn[i],1);
    }
    for (i=0;i<edfh.numofgraphsingals();i++){
        hb2=new QHBoxLayout;
        signalcb[i]=new QCheckBox();
        signalcb[i]->setCheckState(showsignal[i]!=0?Qt::Checked:Qt::Unchecked);
        hb2->addWidget(signalcb[i]);
        edfh.getsignaldata(i,name,NULL,dim,&phmin,&phmax,&digmin,&digmax,NULL,&spr);
        sprintf(str,"Signal %d [%s]",i+1,name);
        sprintf(str2,"Signal %d [%s]\nPhysical %.2f...%.2f%s\nDigital :%d...%d\nSamples per record :%d",i+1,name,phmin,phmax,dim,digmin,digmax,spr);
        QLabel* label=new QLabel();
        if (useTips){
            label->setText(str);
            label->setToolTip(str2);
        }
        else{
            label->setText(str2);
        }
        hb2->addWidget(label);
        hb2->setStretch(hb2->count()-1,1);
        vbn[i%numOfColums]->addLayout(hb2);
    }
    for (j=0;j<(numOfColums-i%numOfColums)%numOfColums;j++)
        vbn[numOfColums-1-j]->addWidget(new QLabel(""));
    hb3=new QHBoxLayout;
    button = new QPushButton("Deselect/Select All");
    QObject::connect(button,&QPushButton::clicked,seldeselall);
    hb3->addWidget(button);
    button = new QPushButton("O.K.");
    QObject::connect(button,&QPushButton::clicked,closesignalswindow);
    hb3->addWidget(button);
    vb->addLayout(hb3);
    signalwp->show();*/
}
