#include "annotationswindowhandler.h"
#include "EDFViewerWindow.h"

AnnotationsWindowHandler::AnnotationsWindowHandler(EDFfilehandler &edfh,EDFViewerWindow &edfViewerWindow):
    QObject(nullptr),
    edfh(edfh),
    edfViewerWindow(edfViewerWindow)
{

}

void AnnotationsWindowHandler::open(){
        QPushButton *button;
        QVBoxLayout *vb = new QVBoxLayout;
        QHBoxLayout *hb2,*hb1,*hb3;
        char annotStr[1024];
        char timeStr[1024];
        char str[256];
        long i,j;

        if (!edfViewerWindow.fileIsOpen())
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
            skipChange=true;
            QObject :: connect (tablewp,&QTableWidget::currentItemChanged,[&]{
                QString str;
                int row=tablewp->currentRow();
                if (skipChange){
                    skipChange=false;
                    return;
                }
                str = tablewp->item(row,0)->text();
                if (str.startsWith("+")){
                    edfViewerWindow.setStartTime(str.toDouble());
                    edfViewerWindow.repaint();
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

