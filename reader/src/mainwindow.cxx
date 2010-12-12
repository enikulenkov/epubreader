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

#include "mainwindow.h"
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "settingsdialog.h"
#include "epublibrarybrowser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_fontSize(20)
{
    setWindowTitle("E-Book Reader");

    QDeclarativeView *view = new QDeclarativeView;
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty("mainWindow", this);
    view->setSource(QUrl("qrc:/qml/epubreader.qml"));

    setCentralWidget(view);

    QAction *openAction = new QAction("Open", this);
    connect(openAction, SIGNAL(triggered()), SLOT(chooseFile()));

    QAction *newWindowAction = new QAction("New Window", this);
    connect(newWindowAction, SIGNAL(triggered()), SIGNAL(newWindow()));

    QAction *libraryAction = new QAction("Library", this);
    connect(libraryAction, SIGNAL(triggered()), SLOT(showLibrary()));

    QAction *settingsAction = new QAction("Settings", this);

    QMenuBar *menu = menuBar();
    menu->addAction(openAction);
    menu->addAction(newWindowAction);
    menu->addAction(libraryAction);
    menu->addAction(settingsAction);

    connect(settingsAction, SIGNAL(triggered()), SLOT(showSettingsDialog()));

    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation);
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

void MainWindow::chooseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "/", "EPUB Files (*.epub)");
    if (!fileName.isNull())
        openFile(fileName);
}

void MainWindow::openFile(QString newFileName)
{
    if (newFileName != m_fileName) {
        m_fileName = newFileName;
        emit fileNameChanged();
    }
}

QString MainWindow::fileName() const
{
    return m_fileName;
}

bool MainWindow::fullSize() const
{
    return windowState() & Qt::WindowFullScreen;
}

void MainWindow::setFullSize(bool set)
{
    bool isFullSize = fullSize();
    if (set != isFullSize) {
        if (set)
            showFullScreen();
        else
            showNormal();
        emit fullSizeChanged();
    }
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog *dlg = new SettingsDialog(this);

    dlg->setFontSize(m_fontSize);
    if (dlg->exec())
        setFontSize(dlg->fontSize());
    delete dlg;
}

int MainWindow::fontSize() const
{
    return m_fontSize;
}

void MainWindow::setFontSize(int size)
{
    if (size != m_fontSize) {
        m_fontSize = size;
        emit fontSizeChanged();
    }
}

void MainWindow::showLibrary()
{
    EPUBLibraryBrowser *win = new EPUBLibraryBrowser(this);
    win->setWindowModality(Qt::WindowModal);

#ifdef Q_WS_MAEMO_5
    win->showMaximized();
#else
    win->setGeometry(100, 100, 800, 480);
    win->show();
#endif
}