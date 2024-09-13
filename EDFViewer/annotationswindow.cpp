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
//#include<qdebug.h>

QWidget *annotationswp;
QTableWidget *tablewp;
char skipChange;

void annotationswindow(){
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb2,*hb1,*hb3;
    char annotStr[1024];
    char timeStr[1024];
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
        tablewp=new QTableWidget();
        tablewp->setColumnCount(2);
        tablewp->verticalHeader()->setVisible(false);
        tablewp->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablewp->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablewp->setSelectionMode(QAbstractItemView::SingleSelection);
        tablewp->setRowCount(0);
        j=0;
        tablewp->setHorizontalHeaderLabels({"Time","Annotation"});
        for (i=0;i<edfh.numofdatarecords;i++){
            edfh.getannotation(i,1,annotStr,timeStr);
            if (annotStr[0]!=0){
                tablewp->setRowCount(j+1);
                tablewp->setItem(j,0, new QTableWidgetItem(timeStr));
                tablewp->setItem(j++,1, new QTableWidgetItem(annotStr));
                }
            }
        vb->addWidget(tablewp);
        skipChange=1;
        QObject :: connect (tablewp,&QTableWidget::currentItemChanged,[&]{
            QString str;
            int row=tablewp->currentRow();
            if (skipChange){
                skipChange=0;
                return;
            }
            str = tablewp->item(row,0)->text();
            if (str.startsWith("+")){
                starttime=str.toDouble();
                edfViewerWindow->repaint();
                }
            });
        }
    hb3=new QHBoxLayout;
    button = new QPushButton("O.K.");
    QObject::connect(button,SIGNAL(clicked()),annotationswp,SLOT(close()));
    hb3->addWidget(button);
    vb->addLayout(hb3);
    annotationswp->show();
}
