#ifndef ANNOTATIONSWINDOWHANDLER_H
#define ANNOTATIONSWINDOWHANDLER_H

#include <QtWidgets>

class EDFViewerWindow;
class EDFfilehandler;

class AnnotationsWindowHandler : public QObject
{
    Q_OBJECT
public:
    explicit AnnotationsWindowHandler(
        EDFfilehandler &edfh,
        EDFViewerWindow &edfViewerWindow);
    void open();

private:
    EDFfilehandler &edfh;
    EDFViewerWindow &edfViewerWindow;

    QWidget *annotationswp;
    QTableWidget *tablewp;
    bool skipChange;
};

#endif // ANNOTATIONSWINDOWHANDLER_H
