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
