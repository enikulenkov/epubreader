#ifndef EPUBURLSCHEMEHANDLER_H
#define EPUBURLSCHEMEHANDLER_H

#include <QWebEngineUrlSchemeHandler>
#include "epubview.h"

class EPUBUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    EPUBUrlSchemeHandler(EPUBView *view);
    void requestStarted(QWebEngineUrlRequestJob *) override;

private:
    EPUBView *m_view;
};

#endif // EPUBURLSCHEMEHANDLER_H
