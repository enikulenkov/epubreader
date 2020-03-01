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

#include <QWebEngineView>

class EPUBFile;

class EPUBView : public QWebEngineView {
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE openFile)
    Q_PROPERTY(QAction* prevPage READ prevPageAction CONSTANT)
    Q_PROPERTY(QAction* nextPage READ nextPageAction CONSTANT)
    Q_PROPERTY(int preferredWidth READ preferredWidth WRITE setPreferredWidth NOTIFY preferredWidthChanged)
    Q_PROPERTY(int preferredHeight READ preferredHeight WRITE setPreferredHeight NOTIFY preferredHeightChanged)
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier)
    Q_PROPERTY(QString defaultFont READ defaultFont WRITE setDefaultFont)
    Q_PROPERTY(int backgroundIndex READ backgroundIndex WRITE setBackgroundIndex)
    Q_PROPERTY(QByteArray tocDocument READ tocDocument)
public:
    //explicit EPUBView(QGraphicsItem *parent = 0);
    explicit EPUBView();

    QString fileName() const;
    QAction *prevPageAction() const;
    QAction *nextPageAction() const;

    int preferredWidth() const;
    int preferredHeight() const;

    qreal textSizeMultiplier() const;
    void setTextSizeMultiplier(qreal factor);

    QString defaultFont() const;
    void setDefaultFont(const QString &font);

    int backgroundIndex() const;
    void setBackgroundIndex(int idx);

    QByteArray tocDocument() const;

public Q_SLOTS:
    bool openFile(const QString &fileName);

    bool showPrevPage();
    bool showNextPage();

    void setPreferredWidth(int width);
    void setPreferredHeight(int height);

    void openTocDocumentRequest(const QString &path);

Q_SIGNALS:
    void preferredHeightChanged();
    void preferredWidthChanged();

    void badFile();

protected:
    virtual bool sceneEvent(QEvent *event);

private Q_SLOTS:
    void handleUrlChange(const QUrl &url);
    void handleExternalLink(const QUrl &url);

private:
    void resizeContent();

    EPUBFile *m_epub;
    QString m_fileName;

    QAction *m_prevPageAction;
    QAction *m_nextPageAction;


    int m_preferredWidth;
    int m_preferredHeight;

    int m_backgroundIndex;

    Qt::GestureType m_swipeGestureType;
};

#endif
