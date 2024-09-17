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

DataWindowHandler::DataWindowHandler(EDFfilehandler& edfh,EDFViewerWindow &edfViewerWindow)
    : QObject{nullptr},
    edfh(edfh),
    edfViewerWindow(edfViewerWindow)
{

}

void DataWindowHandler::open(){
    char str[1024];
    if (!edfViewerWindow.fileIsOpen())
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

void DataWindowHandler::openAbout(){
    QString firstLineStr = QString("<b>EDF Viewer ")+QString(VERSION)+QString("</b><br>");
    QString lastLineStr="<br><br>(c)2016-2024 Kostas Rotas, email:kostasrotas@yahoo.com";
    QWidget *wp=new QWidget();
    QPushButton *button;
    QVBoxLayout *vb = new QVBoxLayout;
    wp->setAttribute(Qt::WA_DeleteOnClose);
    wp->setWindowModality(Qt::ApplicationModal);
    wp->setWindowTitle("About EDF-Viewer");
    wp->setLayout(vb);
    QString browserText=firstLineStr;
    browserText.append(" Program used to display EDF files.<br>  Released under MIT License (MIT)<br>  Created with QT Creator<br>  See Licences.txt for Licences<br><br>");
    browserText.append("Project page:<br><a href=\"http://www.kostasrotas.mysch.gr/edfviewer\">http://www.kostasrotas.mysch.gr/edfviewer</a><br>");
    browserText.append("Github Page:<br><a href=\"http://github.com/kostasrotas/EDFViewer-Desktop#edfviewer-desktop\">http://github.com/kostasrotas/EDFViewer-Desktop</a>" );
    browserText.append(lastLineStr);
    QTextBrowser* browser = new QTextBrowser();
    browser->setHtml(browserText);
    browser->setOpenExternalLinks( true );
    QFontMetrics m (browser -> font()) ;
    int rowHeight = m.lineSpacing() ;
    browser -> setFixedHeight  (14*rowHeight) ;
    browser -> setFixedWidth(2+m.horizontalAdvance(lastLineStr));
    vb->addWidget(browser);
    wp->show();
    button = new QPushButton("O.K.");
    vb->addWidget(button);
    QObject::connect(button,SIGNAL(clicked()),wp,SLOT(close()));
}
