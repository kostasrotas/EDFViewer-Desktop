#include "signalswindowhandler.h"
#include "EDFViewerWindow.h"

SignalsWindowHandler::SignalsWindowHandler(EDFfilehandler &edfh,EDFViewerWindow &edfViewerWindow):
    QObject(nullptr),
    edfh(edfh),
    edfViewerWindow(edfViewerWindow){
}

void SignalsWindowHandler::closesignalswindow(){
    int i;
    for (i=0;i<edfh.numofgraphsingals();i++)
        edfViewerWindow.setShowSignal(i,signalcb[i]->isChecked());
    signalwp->close();
    edfViewerWindow.update();
    }

void SignalsWindowHandler::seldeselall(){
    int i;
    Qt::CheckState newvalue=Qt::Checked;
    for (i=0;i<edfh.numofgraphsingals();i++)
        if (signalcb[i]->isChecked()){
            newvalue=Qt::Unchecked;
            break;
        }
    for (i=0;i<edfh.numofgraphsingals();i++){
        signalcb[i]->setCheckState(newvalue);
        }
    }

void SignalsWindowHandler::open(){
        char str[256];
        char str2[256];
        char useTips;
        float phmin,phmax;
        int spr,digmin,digmax;
        char name[256],dim[256];
        int i,j;
        int numOfColums=1;
        if (edfViewerWindow.fileIsOpen()==0)
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
            signalcb[i]->setCheckState(edfViewerWindow.getShowSignal(i)!=0?Qt::Checked:Qt::Unchecked);
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
        connect(button,SIGNAL(clicked()),this,SLOT(seldeselall()));
        hb3->addWidget(button);
        button = new QPushButton("O.K.");
        connect(button,SIGNAL(clicked()),this,SLOT(closesignalswindow()));
        hb3->addWidget(button);
        vb->addLayout(hb3);
        signalwp->show();
    }
