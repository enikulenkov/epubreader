/* Copyright © 2010 Євгеній Мещеряков <eugen@debian.org>
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

#include "epubreaderapplication.h"
#include "epubview.h"
#include "mainwindow.h"
#include <QDBusConnection>
#include <QtDeclarative>
#include <QDBusError>

EPUBApplicationDBusAdapter::EPUBApplicationDBusAdapter(EPUBReaderApplication *app) :
    QDBusAbstractAdaptor(app)
{
}

int EPUBApplicationDBusAdapter::mime_open(const QString &s1)
{
    EPUBReaderApplication *app = qobject_cast<EPUBReaderApplication *>(parent());
    Q_ASSERT(app);

    if (!s1.isEmpty()) {
        QUrl fileUrl(s1);
        QString fileName = fileUrl.toLocalFile();
        if (fileName.isEmpty())
            return 0; // FIXME make it possible to open URLs too
        app->openFile(fileName);
    }

    return 0;
}

EPUBReaderApplication::EPUBReaderApplication(int &argc, char**argv) :
    QApplication(argc, argv)
{
    setApplicationName("EPUBReader");

    qmlRegisterType<EPUBView>("EPUBReader", 1, 0, "EPUBView");

    MainWindow *win = new MainWindow;

    // TODO better error handling
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning() << "Cannot connect to session bus.";
        exit(1);
    }

    if (!QDBusConnection::sessionBus().registerService("org.opensource.epubreader.Reader")) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        exit(2);
    }

    EPUBApplicationDBusAdapter *adapter = new EPUBApplicationDBusAdapter(this);

    if (!QDBusConnection::sessionBus().registerObject("/org/opensource/epubreader/Reader", adapter)) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        exit(3);
    }

    showNewWindow(win);
}

void EPUBReaderApplication::openFile(const QString &fileName)
{
    /* First try to find an existing window without open file */
    foreach (QWidget *w, topLevelWidgets()) {
        MainWindow *win = qobject_cast<MainWindow *>(w);
        if (win && win->fileName().isEmpty()) {
            win->openFile(fileName);
            win->activateWindow();
            return;
        }
    }

    /* If an empty window was not found, create new one */
    MainWindow *win = new MainWindow;
    win->openFile(fileName);
    showNewWindow(win);
}

void EPUBReaderApplication::openNewWindow()
{
    MainWindow *win = new MainWindow;
    showNewWindow(win);
}

void EPUBReaderApplication::showNewWindow(MainWindow *win)
{
    connect(win, SIGNAL(newWindow()), SLOT(openNewWindow()));

#ifdef Q_WS_MAEMO_5
    win->showMaximized();
#else
    win->setGeometry(100, 100, 800, 480);
    win->show();
#endif
}