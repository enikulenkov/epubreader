#ifndef EPUBURLSCHEMEHANDLER_H
#define EPUBURLSCHEMEHANDLER_H

#include <QWebEngineUrlSchemeHandler>
#include "epubfile.h"

class EPUBUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    EPUBUrlSchemeHandler();
    void requestStarted(QWebEngineUrlRequestJob *) override;
    void setFile(EPUBFile &file);

private:
    EPUBFile *m_file;
};

#endif // EPUBURLSCHEMEHANDLER_H
