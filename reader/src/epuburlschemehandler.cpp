#include "epuburlschemehandler.h"
#include <QWebEngineUrlRequestJob>
#include <QNetworkAccessManager>
#include "epubreply.h"

EPUBUrlSchemeHandler::EPUBUrlSchemeHandler() :
    m_file(0)
{
}

void EPUBUrlSchemeHandler::setFile(EPUBFile &file)
{
    m_file = &file;
}

void EPUBUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *job)
{
    Q_ASSERT(m_file);
    EPUBReply *reply = new EPUBReply(m_file, QNetworkAccessManager::GetOperation, job);

    reply->processRequest();

    job->reply(reply->mimeType(), reply);
}
