#ifndef EPUBURLSCHEMEHANDLER_H
#define EPUBURLSCHEMEHANDLER_H

#include <QWebEngineUrlSchemeHandler>

class EPUBUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    EPUBUrlSchemeHandler();
    void requestStarted(QWebEngineUrlRequestJob *) override;
};

#endif // EPUBURLSCHEMEHANDLER_H
