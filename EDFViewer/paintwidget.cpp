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

char drawgrid=1;

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent){

}


void PaintWidget::drawsignal(QPainter *p,int s,int x1,int y1,int x2,int y2){
    int firstsample;
    int samplestoprint;
    int posx,posy,lastposx,lastposy;
    int digmin,digmax;
    char name[256];
    char dimension[256];
    float phmin,phmax;
    char str[256];
    int i,ii,j;
    edfh.getsignaldata(s,name,NULL,dimension,&phmin,&phmax,&digmin,&digmax,NULL,NULL);
    samplestoprint=screentime/edfh.recduration*edfh.samplesperrecord[s];
    firstsample=starttime/edfh.recduration*edfh.samplesperrecord[s];
    p->setPen(QPen(0x999999));
    p->drawLine(x1,y1,x1,y2);
    p->drawLine(x1,y2,x2,y2);
    sprintf(str,"%.2f %s",phmin,dimension);
    p->drawText(x1+2,y2-2,str);
    sprintf(str,"%.2f %s",phmax,dimension);
    p->drawText(x1+2,y1+10,str);
    p->setPen(QPen(0x105810));//green lines
    j=10;
    if (drawgrid)
        for (int i=0;i<j;i++){//vertical lines
            ii=((x2-x1)/j)*(i+1);
            p->drawLine(x1+ii,y1,x1+ii,y2);
        }
    if (phmin<0 && phmax>0){//zeroline
        int zy=((y2-y1)*(-phmin))/(phmax-phmin);
        int dy=((y2-y1)/10)*(i+1);
        j=zy;
        ii=1;
        if (drawgrid)
            if (dy>4){//hor lines
                while (zy+ii*dy<y2-y1){
                    p->drawLine(x1,y2-(zy+ii*dy),x2,y2-(zy+ii*dy));
                    ii++;
                }
                ii=1;
                while (zy-ii*dy>0){
                    p->drawLine(x1,y2-(zy-ii*dy),x2,y2-(zy-ii*dy));
                    ii++;
                }
            }
        p->setPen(QPen(0xa2f8a2));
        p->drawLine(x1,y2-zy,x2,y2-zy);//zero line
    }
    else if (drawgrid){// hor lines , no zero line
        for (int i=0;i<10;i++){
            ii=((y2-y1)/10)*(i+1);
            p->drawLine(x1,y2-ii,x2,y2-ii);
        }
    }
    p->setPen(QPen(0xdddddd));
    p->drawText(x1+60,y1+10,name);
    for (int i=0;i<samplestoprint;i++){
        int y2zero;
        posx=x1+(i*(x2-x1))/samplestoprint;
        y2zero=y2+digmin*(y2-y1)/(digmax-digmin);
        posy=y2zero-(edfh.getdata(s,firstsample+i)*(y2-y1))/((digmax-digmin)/yZoom);
        if (i!=0){
            p->drawLine(posx,posy,lastposx,lastposy);
        }
        lastposx=posx;
        lastposy=posy;
    }
}

void PaintWidget::paintEvent(QPaintEvent *event){
    int d,i,ii;
    QPainter painter;

    painter.begin(this);
    painter.fillRect(rect(),Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    if (fileisopen){
        int numstoprint=0,i;
        for (i=0;i<edfh.numofgraphsingals();i++)
            numstoprint+=showsignal[i];
        if (numstoprint==0){
            showsignal[0]=1;
            numstoprint=1;
        }
        d=height()/numstoprint;
        ii=0;
        for (i=0;i<edfh.numofgraphsingals();i++)
            if (showsignal[i]){
                drawsignal(&painter,i,10,ii*d+10,width()-10,(ii+1)*(d)-10);
                ii++;
            }
        painter.setPen(QPen(0xdddddd));
        char str[256];
        sprintf(str,"  %5.1f sec",starttime);
        painter.drawText(2,height()-2,str);
    }
    painter.end();
}

PaintWidget::~PaintWidget(){

}

