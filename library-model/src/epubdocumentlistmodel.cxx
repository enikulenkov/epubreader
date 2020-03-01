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

#include "epubdocumentlistmodel.h"
#include "search_interface.h"
#include <QDebug>

#define EPUB_QUERY \
    "<rdfq:Condition>" \
      "<rdfq:equals>" \
        "<rdfq:Property name=\"File:Mime\"/>" \
        "<rdf:String>application/epub+zip</rdf:String>" \
      "</rdfq:equals>" \
    "</rdfq:Condition>"

EPUBDocumentListModel::EPUBDocumentListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_search = new OrgFreedesktopTrackerSearchInterface(QLatin1String("org.freedesktop.Tracker"),
        QLatin1String("/org/freedesktop/Tracker/Search"),
        QDBusConnection::sessionBus(), this);

    QDBusPendingReply<QList<QStringList>> reply = m_search->Query(-1, QLatin1String("Files"),
                                   QStringList() << QLatin1String("DC:Title") << QLatin1String("DC:Creator"),
                                   QLatin1String(""),
                                   QStringList(),
                                   QLatin1String(EPUB_QUERY),
                                   false,
                                   QStringList(),
                                   false, 0, -1);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(callFinished(QDBusPendingCallWatcher*)));

#ifndef Q_WS_MAEMO_5
    // Add some dummy data
    m_data << EPUBDesc(QLatin1String("/nonexistent.epub"),
                       QLatin1String("Sample Book"),
                       QLatin1String("An Author"));
    m_data << EPUBDesc(QLatin1String("/nonexistent1.epub"),
                       QLatin1String("Sample Book 2"),
                       QLatin1String("Another Author"));
#endif
}

int EPUBDocumentListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

QVariant EPUBDocumentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.parent().isValid())
        return QVariant();
    if (index.column() != 0)
        return QVariant();
    if (index.row() >= m_data.size())
        return QVariant();

    int idx = index.row();
    if (role == Qt::DisplayRole)
        return m_data.at(idx).title;
    else if (role == FileNameRole)
        return m_data.at(idx).fileName;
    else if (role == AuthorRole)
        return m_data.at(idx).author;
    return QVariant();
}

void EPUBDocumentListModel::callFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QList<QStringList>> reply = *call;

    if (reply.isError()) {
        qWarning() << "Query call error";
    } else {
        beginResetModel();
        m_data.clear();

        QList<QStringList> list = reply.argumentAt<0>();
        Q_FOREACH (const QStringList &l, list) {
            if (l.length() < 4) {
                qWarning() << "Wrong entry length";
                continue;
            }

            m_data << EPUBDesc(l.at(0), l.at(2), l.at(3));
        }
        endResetModel();
    }

    call->deleteLater();
}

QHash<int, QByteArray> EPUBDocumentListModel::roleNames()
{
    QHash<int, QByteArray> roleNames;

    roleNames[Qt::DisplayRole] = "display";
    roleNames[EPUBDocumentListModel::FileNameRole] = "fileName";
    roleNames[EPUBDocumentListModel::AuthorRole] = "author";
    return roleNames;
}
