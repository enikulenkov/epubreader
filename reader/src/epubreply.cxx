/* Copyright © 2010-2011 Євгеній Мещеряков <eugen@debian.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "epubreply.h"
#include <QNetworkAccessManager>
#include <QTimer>
#include <QDebug>

EPUBReply::EPUBReply(EPUBFile *epub, QNetworkAccessManager::Operation op, const QWebEngineUrlRequestJob *job, QObject *parent) :
    QIODevice(parent), m_epub(epub), m_position(0)
{
    (void)op;
    m_job = job;
    QTimer::singleShot(0, this, SLOT(processRequest()));
    qDebug() << "Loading" << job->requestUrl().toString();
}

void EPUBReply::processRequest()
{
    if (!m_epub) {
#if 0
        setError(ConnectionRefusedError, QLatin1String("File is not opened"));
        Q_EMIT error(ConnectionRefusedError);
        Q_EMIT finished();
        return;
#endif
    }

#if 0
    if (operation() != QNetworkAccessManager::GetOperation) {
        setError(ProtocolInvalidOperationError, QLatin1String("Operation is not supported"));
        Q_EMIT error(ProtocolInvalidOperationError);
        Q_EMIT finished();
        return;
    }
#endif

    QUrl url = m_job->requestUrl();

    if (url.scheme() != QLatin1String("epub")) {
#if 0
        setError(ProtocolUnknownError, QLatin1String("Unsupported scheme"));
        Q_EMIT error(ProtocolUnknownError);
        Q_EMIT finished();
#endif
        return;
    }

    m_content = m_epub->getFileByUrl(url, &m_mimeType);

    if (m_content.isNull()) {
#if 0
        setError(ContentNotFoundError, QLatin1String("File not found"));
        Q_EMIT error(ContentNotFoundError);
        Q_EMIT finished();
#endif
        return;
    }

    open(ReadOnly | Unbuffered);
#if 0
    setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    setHeader(QNetworkRequest::ContentLengthHeader, m_content.size());
#endif

    Q_EMIT readyRead();
    //Q_EMIT finished();
}

bool EPUBReply::isSequential() const
{
    return true;
}

const QByteArray EPUBReply::mimeType()
{
    return m_mimeType.toUtf8();
}

qint64 EPUBReply::bytesAvailable() const
{
    return m_content.size() - m_position;
}

qint64 EPUBReply::readData(char *data, qint64 maxlen)
{
    if (m_position < m_content.size()) {
        qint64 num = qMin(maxlen, m_content.size() - m_position);
        memcpy(data, m_content.constData() + m_position, num);
        m_position += num;
        return num;
    }
    return -1;
}

qint64 EPUBReply::writeData(const char *data, qint64 maxlen)
{
    (void)data;
    (void)maxlen;
    return 0;
}
