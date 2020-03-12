#include "epubreaderapplication.h"
#include <QWebEngineUrlScheme>

void registerEpubUrlScheme()
{
    QWebEngineUrlScheme scheme("epub");
    scheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);
    scheme.setFlags(QWebEngineUrlScheme::LocalScheme | QWebEngineUrlScheme::LocalAccessAllowed);
    QWebEngineUrlScheme::registerScheme(scheme);
}

int main(int argc, char **argv)
{
    registerEpubUrlScheme();
    EPUBReaderApplication app(argc, argv);

    return app.exec();
}
