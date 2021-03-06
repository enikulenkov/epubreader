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

#ifndef EPUBTHUMBNAILERREQUESTQUEUE_H
#define EPUBTHUMBNAILERREQUESTQUEUE_H

#include <QObject>
#include <QQueue>

class EPUBThumbnailerRequest;
class QTimer;

class EPUBThumbnailerRequestQueue : public QObject {
    Q_OBJECT
public:
    explicit EPUBThumbnailerRequestQueue(QObject *parent = 0);

    uint enqueue(const QStringList &uris);

Q_SIGNALS:
    void ready(const QString &uri);
    void started(uint handle);
    void finished(uint handle);
    void error(const QString &uri, int errorCode, const QString &message);

    void requestQueueEmpty();

private Q_SLOTS:
    void processRequest();
    void timeout();

private:
    uint m_requestCounter;
    QQueue<EPUBThumbnailerRequest *> m_requests;
    bool m_busy;

    QTimer *m_timeoutTimer;
};

#endif
