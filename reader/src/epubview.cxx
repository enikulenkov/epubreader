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

#include "epubview.h"
#include "epubaccessmanager.h"
#include "epubfile.h"
#include <QAction>
#include <QWebFrame>
#include <QDebug>
#include <QWebSecurityOrigin>
#include <QDesktopServices>
#include "epubreaderapplication.h"
#include "epubreadersettings.h"
#include "horizmouseswipegesture.h"
#include "desktopnotifications.h"

#define STYLESHEET_TEMPLATE \
    "html {" \
        "background: %1 !important;" \
        "margin-top: 0px !important" \
    "}" \
    "body {" \
        "background: transparent !important;" \
        "margin-top: 10px !important;" \
        "margin-bottom: 10px !important;" \
        "margin-right: 10px !important;" \
        "margin-left: 10px !important" \
    "}"

EPUBView::EPUBView(QGraphicsItem *parent) :
    QGraphicsWebView(parent), m_epub(0), m_preferredWidth(800), m_preferredHeight(600)
{
    QWebSettings *s = settings();

    s->setAttribute(QWebSettings::JavascriptEnabled, false);
    s->setAttribute(QWebSettings::JavaEnabled, false);
    s->setAttribute(QWebSettings::FrameFlatteningEnabled, true);
    s->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);

    QWebFrame *frame = page()->mainFrame();
    frame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    frame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    page()->setNetworkAccessManager(new EPUBAccessManager);
    page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);

    m_prevPageAction = new QAction(this);
    m_nextPageAction = new QAction(this);

    connect(m_prevPageAction, SIGNAL(triggered()), SLOT(showPrevPage()));
    connect(m_nextPageAction, SIGNAL(triggered()), SLOT(showNextPage()));

    m_prevPageAction->setEnabled(false);
    m_nextPageAction->setEnabled(false);

    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(handleUrlChange(QUrl)));

    QWebSecurityOrigin::addLocalScheme(QLatin1String("epub"));
    connect(this, SIGNAL(linkClicked(QUrl)), SLOT(handleExternalLink(QUrl)));

    setResizesToContents(true);

    setBackgroundIndex(0);

    m_swipeGestureType = EPUBReaderApplication::swipeGestureType();
    grabGesture(m_swipeGestureType);
}

int EPUBView::backgroundIndex() const
{
    return m_backgroundIndex;
}

void EPUBView::setBackgroundIndex(int idx)
{
    m_backgroundIndex = idx;
    QColor c;

    switch (m_backgroundIndex) {
    default:
        c = QColor(Qt::white);
        break;
    case 1:
        c = QColor(0xf1, 0xdc, 0x6b);
        break;
    case 2:
        c = QColor(Qt::gray);
        break;
    }

    QString style = QString::fromLatin1(STYLESHEET_TEMPLATE).arg(c.name());
    QByteArray userStyleSheet = style.toUtf8();
    QByteArray url = QByteArray("data:text/css;charset=utf-8;base64,") + userStyleSheet.toBase64();
    settings()->setUserStyleSheetUrl(QUrl::fromEncoded(url));
    resizeContent();
}

bool EPUBView::openFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;

    EPUBReaderSettings *settings = EPUBReaderApplication::settings();
    settings->saveLastURL(m_fileName, url());

    Q_ASSERT(qobject_cast<EPUBAccessManager *>(page()->networkAccessManager()));
    EPUBAccessManager *manager = static_cast<EPUBAccessManager *>(page()->networkAccessManager());

    EPUBFile *newEPUB = new EPUBFile(fileName, this);
    if (newEPUB->status() == EPUBFile::NoError) {
        m_prevPageAction->setEnabled(false);
        m_nextPageAction->setEnabled(false);

        if (m_epub) {
            manager->setDocument(0);
            delete m_epub;
        }

        m_epub = newEPUB;

        manager->setDocument(m_epub);
        m_fileName = fileName;

        /* open last viewed page or default one if not found */
        QUrl lastUrl = settings->lastUrlForFile(fileName);
        if (!lastUrl.isValid() || !m_epub->hasUrl(lastUrl)) {
            QUrl defaultUrl = m_epub->defaultUrl();
            load(defaultUrl);
        } else
            load(lastUrl);

        return true;
    }

    delete newEPUB;

    //setContent("No file"); // TODO add some error page
    // TODO show an error page and exit
    return false;
}

QString EPUBView::fileName() const
{
    return m_fileName;
}

QAction *EPUBView::prevPageAction() const
{
    return m_prevPageAction;
}

QAction *EPUBView::nextPageAction() const
{
    return m_nextPageAction;
}

bool EPUBView::showPrevPage()
{
    if (url().scheme() != QLatin1String("epub"))
        return false; // TODO maybe just try to show first page
    if (!m_epub)
        return false;
    QUrl newUrl = m_epub->getPrevPage(url()); // TODO check if page is the same
    load(newUrl);
    return true;
}

bool EPUBView::showNextPage()
{
    if (url().scheme() != QLatin1String("epub"))
        return false; // TODO maybe try to show last page
    if (!m_epub)
        return false;
    QUrl newUrl = m_epub->getNextPage(url()); // TODO check if page is the same
    load(newUrl);
    return true;
}

void EPUBView::handleUrlChange(const QUrl &url)
{
    if (url.scheme() != QLatin1String("epub")) {
        m_prevPageAction->setEnabled(false);
        m_nextPageAction->setEnabled(false);
    } else {
        EPUBFile::PageFlags flags = m_epub->getUrlInfo(url);
        m_prevPageAction->setEnabled(flags & EPUBFile::PageHasPrevious);
        m_nextPageAction->setEnabled(flags & EPUBFile::PageHasNext);
    }
}

int EPUBView::preferredWidth() const
{
    return m_preferredWidth;
}

int EPUBView::preferredHeight() const
{
    return m_preferredHeight;
}

void EPUBView::setPreferredWidth(int width)
{
    if (width != m_preferredWidth) {
        m_preferredWidth = width;
        resizeContent();
        Q_EMIT preferredWidthChanged();
    }
}

void EPUBView::setPreferredHeight(int height)
{
    if (height != m_preferredHeight) {
        m_preferredHeight = height;
        resizeContent();
        Q_EMIT preferredHeightChanged();
    }
}

void EPUBView::resizeContent()
{
    page()->setPreferredContentsSize(QSize(m_preferredWidth, m_preferredHeight));
    setTextSizeMultiplier(textSizeMultiplier()); // FIXME otherwise content may not feet view
}

qreal EPUBView::textSizeMultiplier() const
{
    return page()->mainFrame()->textSizeMultiplier();
}

void EPUBView::setTextSizeMultiplier(qreal factor)
{
    page()->mainFrame()->setTextSizeMultiplier(factor);
}

QString EPUBView::defaultFont() const
{
    return settings()->fontFamily(QWebSettings::StandardFont);
}

void EPUBView::setDefaultFont(const QString &font)
{
    settings()->setFontFamily(QWebSettings::StandardFont, font);
    resizeContent();
}

QByteArray EPUBView::tocDocument() const
{
    if (m_epub)
        return m_epub->tocDocument();
    return QByteArray();
}

void EPUBView::openTocDocumentRequest(const QString &path)
{
    if (!m_epub)
        return;

    load(m_epub->resolveTocUrl(QUrl(path)));
}

bool EPUBView::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::Gesture) {
        QGestureEvent *ge = static_cast<QGestureEvent *>(event);
        HorizMouseSwipeGesture *g = static_cast<HorizMouseSwipeGesture *>(ge->gesture(m_swipeGestureType));
        if (g->state() == Qt::GestureFinished) {
            if (!g->left()) {
                if (m_prevPageAction->isEnabled())
                    showPrevPage();
                else
                    DesktopNotifications::showInfoprint(tr("Already at first page"));
            } else {
                if (m_nextPageAction->isEnabled())
                    showNextPage();
                else
                    DesktopNotifications::showInfoprint(tr("Already at last page"));
            }
        }
        return true;
    }

    return QGraphicsWebView::sceneEvent(event);
}

void EPUBView::handleExternalLink(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}
