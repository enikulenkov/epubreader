#include "epuburlschemehandler.h"

EPUBUrlSchemeHandler::EPUBUrlSchemeHandler()
{

}

//QNetworkReply *EPUBAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
//{
    //Q_UNUSED(outgoingData);
    //return new EPUBReply(m_epub, op, request);
//}

void EPUBUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *)
{
    Q_ASSERT(0);
}
