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
#include "epubfile.h"
#include <QAction>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QDebug>
#include <QQuickWebEngineProfile>
#include <QDesktopServices>
#include "epubreaderapplication.h"
#include "epubreadersettings.h"
#include "desktopnotifications.h"
#include "epuburlschemehandler.h"
#include "epubfile.h"

EPUBView::EPUBView() :
    m_epub(0), m_textSizeMultiplier(1), m_schemeHandler()
{

#if 0
    QWebFrame *frame = page()->mainFrame();
    frame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    frame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
#endif

    m_prevPageAction = new QAction(this);
    m_nextPageAction = new QAction(this);

    connect(m_prevPageAction, SIGNAL(triggered()), SLOT(showPrevPage()));
    connect(m_nextPageAction, SIGNAL(triggered()), SLOT(showNextPage()));

    m_prevPageAction->setEnabled(false);
    m_nextPageAction->setEnabled(false);

    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(handleUrlChange(QUrl)));

    setBackgroundIndex(0);
}

int EPUBView::backgroundIndex() const
{
    return m_backgroundIndex;
}

void EPUBView::setBackgroundIndex(int idx)
{
    if (m_backgroundIndex != idx) {
        m_backgroundIndex = idx;
        resizeContent();
        Q_EMIT backgroundIndexChanged(idx);
    }
}

bool EPUBView::setUrl(const QUrl &url)
{
    m_url = url;
    Q_EMIT urlChanged(url);
    return true;
}

void EPUBView::installEpubUrlSchemeHandler()
{
    m_schemeHandler.setFile(*m_epub);
    QQuickWebEngineProfile::defaultProfile()->installUrlSchemeHandler("epub", &m_schemeHandler);
}

bool EPUBView::openFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;

    EPUBReaderSettings *settings = EPUBReaderApplication::settings();
    settings->saveLastURL(m_fileName, url());

    EPUBFile *newEPUB = new EPUBFile(fileName, this);
    if (newEPUB->status() == EPUBFile::NoError) {
        m_prevPageAction->setEnabled(false);
        m_nextPageAction->setEnabled(false);

        if (m_epub) {
            delete m_epub;
        }

        m_epub = newEPUB;
        this->installEpubUrlSchemeHandler();

        m_fileName = fileName;

        /* open last viewed page or default one if not found */
        QUrl lastUrl = settings->lastUrlForFile(fileName);
        if (!lastUrl.isValid() || !m_epub->hasUrl(lastUrl)) {
            QUrl defaultUrl = m_epub->defaultUrl();
            setUrl(defaultUrl);
            Q_EMIT urlChanged(defaultUrl);
        } else {
            setUrl(lastUrl);
            Q_EMIT urlChanged(lastUrl);
        }
        Q_EMIT pagesNumChanged(pagesNum());
        Q_EMIT documentOpened();

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

QUrl EPUBView::url() const
{
    return m_url;
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
    setUrl(newUrl);
    return true;
}

bool EPUBView::showNextPage()
{
    if (url().scheme() != QLatin1String("epub"))
        return false; // TODO maybe try to show last page
    if (!m_epub)
        return false;
    QUrl newUrl = m_epub->getNextPage(url()); // TODO check if page is the same
    setUrl(newUrl);
    return true;
}

void EPUBView::showPage(int idx)
{
    if (m_epub) {
        QUrl newUrl = m_epub->getSpineUrl(idx);
        setUrl(newUrl);
    }
}

QUrl EPUBView::getPageUrl(int pageNum)
{
    assert(m_epub != NULL);
    return m_epub->getSpineUrl(pageNum);
}

int EPUBView::getPageIdx(const QUrl &url) const
{
    assert(m_epub != NULL);
    return m_epub->getSpineIdx(url);
}

int EPUBView::pagesNum() const
{
    if (m_epub) {
        return m_epub->spineSize();
    }
    return 0;
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

void EPUBView::resizeContent()
{
    setTextSizeMultiplier(textSizeMultiplier()); // FIXME otherwise content may not feet view
}

qreal EPUBView::textSizeMultiplier() const
{
    return m_textSizeMultiplier;
}

void EPUBView::setTextSizeMultiplier(qreal factor)
{
    if (!qFuzzyCompare(m_textSizeMultiplier, factor)) {
        m_textSizeMultiplier = factor;
        Q_EMIT textSizeMultiplierChanged(factor);
    }
}

QString EPUBView::defaultFont() const
{
    return m_defaultFont;
}

void EPUBView::setDefaultFont(const QString &font)
{
    if (m_defaultFont != font) {
        m_defaultFont = font;
        resizeContent();
        Q_EMIT defaultFontChanged(font);
    }
}

QByteArray EPUBView::tocDocument() const
{
    if (m_epub)
        return m_epub->tocDocument();
    return QByteArray();
}

void EPUBView::openTocDocumentRequest(const QString &path)
{
    (void)path;
    if (!m_epub)
        return;

    setUrl(m_epub->resolveTocUrl(QUrl(path)));
}
