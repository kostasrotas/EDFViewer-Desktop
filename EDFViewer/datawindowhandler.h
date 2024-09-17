#ifndef DATAWINDOWHANDLER_H
#define DATAWINDOWHANDLER_H

#include <QtWidgets>

class EDFViewerWindow;
class EDFfilehandler;

class DataWindowHandler : public QObject
{
    Q_OBJECT
public:
    explicit DataWindowHandler(EDFfilehandler&,
                               EDFViewerWindow &);
    void open();
    void openAbout();

signals:

private:
    EDFfilehandler &edfh;
    EDFViewerWindow &edfViewerWindow;

    void aboutwindow();
};

#endif // DATAWINDOWHANDLER_H
