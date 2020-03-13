#include "epuburlschemehandler.h"
#include <QWebEngineUrlRequestJob>
#include <QNetworkAccessManager>
#include "epubreply.h"

EPUBUrlSchemeHandler::EPUBUrlSchemeHandler(EPUBView *view)
{
    m_view = view;
}

void EPUBUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *job)
{
    EPUBReply *reply = new EPUBReply(m_view->epubFile(), QNetworkAccessManager::GetOperation, job);

    reply->processRequest();

    job->reply(reply->mimeType(), reply);
}
