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

#ifndef EPUBREPLY_H
#define EPUBREPLY_H

#include <QWebEngineUrlRequestJob>
#include <QNetworkAccessManager>
#include <QPointer>
#include "epubfile.h"

class EPUBReply : public QIODevice {
    Q_OBJECT
public:
    explicit EPUBReply(EPUBFile *epub, QNetworkAccessManager::Operation op, const QWebEngineUrlRequestJob *job, QObject *parent = 0);
    virtual bool isSequential() const;
    virtual qint64 bytesAvailable() const;
    const QByteArray mimeType();
Q_SLOT
    void processRequest();
protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 maxlen);
private:
    QPointer<EPUBFile> m_epub;
    QByteArray m_content;
    QString m_mimeType;
    qint64 m_position;
    const QWebEngineUrlRequestJob *m_job;
};

#endif
