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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "epubreaderapplication.h"
#include "epubreadersettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QComboBox *cb = ui->backgroundComboBox;
    cb->addItem(tr("White"));
    cb->setItemData(0, QColor(Qt::white), Qt::DecorationRole);
    cb->addItem(tr("Yellow"));
    cb->setItemData(1, QColor(0xF1, 0xDC, 0x6B), Qt::DecorationRole);
    cb->addItem(tr("Gray"));
    cb->setItemData(2, QColor(Qt::gray), Qt::DecorationRole);

    EPUBReaderSettings *settings = qobject_cast<EPUBReaderApplication *>(QCoreApplication::instance())->settings();
    ui->fontSizeSlider->setValue(settings->textSizeMultiplier() * 10);
    ui->fontComboBox->setCurrentFont(settings->fontFamily());
    ui->backgroundComboBox->setCurrentIndex(settings->colorIndex());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    EPUBReaderSettings *settings = qobject_cast<EPUBReaderApplication *>(QCoreApplication::instance())->settings();
    settings->setTextSizeMultiplier(ui->fontSizeSlider->value() / 10.0);
    settings->setFontFamily(ui->fontComboBox->currentFont().family());
    settings->setColorIndex(ui->backgroundComboBox->currentIndex());
    QDialog::accept();
}
