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

#ifndef EPUBVIEW_H
#define EPUBVIEW_H

#include <QObject>
#include <QAction>
#include <QUrl>
#include "epuburlschemehandler.h"

class EPUBFile;

class EPUBView : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE openFile)
    Q_PROPERTY(QAction* prevPage READ prevPageAction CONSTANT)
    Q_PROPERTY(QAction* nextPage READ nextPageAction CONSTANT)
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier NOTIFY textSizeMultiplierChanged)
    Q_PROPERTY(QString defaultFont READ defaultFont WRITE setDefaultFont NOTIFY defaultFontChanged)
    Q_PROPERTY(int backgroundIndex READ backgroundIndex WRITE setBackgroundIndex NOTIFY backgroundIndexChanged)
    Q_PROPERTY(QByteArray tocDocument READ tocDocument)
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
    Q_PROPERTY(int pagesNum READ pagesNum NOTIFY pagesNumChanged)
public:
    explicit EPUBView();

    QString fileName() const;
    QUrl url() const;
    QAction *prevPageAction() const;
    QAction *nextPageAction() const;

    qreal textSizeMultiplier() const;
    void setTextSizeMultiplier(qreal factor);

    QString defaultFont() const;
    void setDefaultFont(const QString &font);

    int backgroundIndex() const;
    void setBackgroundIndex(int idx);

    QByteArray tocDocument() const;
    int pagesNum() const;

public Q_SLOTS:
    bool openFile(const QString &fileName);
    bool setUrl(const QUrl &url);

    bool showPrevPage();
    bool showNextPage();
    void showPage(int idx);
    QUrl getPageUrl(int pageNum);
    int getPageIdx(const QUrl &url) const;

    void openTocDocumentRequest(const QString &path);

Q_SIGNALS:
    void urlChanged(const QUrl &url);
    void textSizeMultiplierChanged(qreal factor);
    void defaultFontChanged(QString defaultFont);
    void backgroundIndexChanged(int index);
    void badFile();
    void pagesNumChanged(int pagesNum);
    void documentOpened();

protected:
    virtual bool sceneEvent(QEvent *event);

private Q_SLOTS:
    void handleUrlChange(const QUrl &url);
    void handleExternalLink(const QUrl &url);

private:
    void resizeContent();
    void installEpubUrlSchemeHandler();

    QUrl m_url;
    EPUBFile *m_epub;
    QString m_fileName;
    qreal m_textSizeMultiplier;
    QString m_defaultFont;

    QAction *m_prevPageAction;
    QAction *m_nextPageAction;

    int m_backgroundIndex;

    EPUBUrlSchemeHandler m_schemeHandler;

    Qt::GestureType m_swipeGestureType;
};

#endif
