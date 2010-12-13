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

#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include "epubfile.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (app.arguments().count() < 2) {
        std::cerr << "Usage: " << qPrintable(app.arguments()[0]) << " <file.epub>\n";
        return 1;
    }

    EPUBFile file(app.arguments()[1]);
    if (file.status() != EPUBFile::NoError)
        return 2;

    std::cout << qPrintable(file.metadata());
    std::cout << "тест\n";
    return 0;
}
