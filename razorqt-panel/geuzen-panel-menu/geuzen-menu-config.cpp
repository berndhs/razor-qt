/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Maciej PÃÂaza <plaza.maciej@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#include "geuzen-menu-config.h"
#include "ui_geuzen-menu-config.h"
#include <qtxdg/xdgmenu.h>

#include <QtGui/QFileDialog>

GeuzenMenuConfig::GeuzenMenuConfig(QSettings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeuzenMenuConfig),
    mSettings(settings),
    mOldSettings(settings)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName("MainMenuConfigurationWindow");
    ui->setupUi(this);

    connect(ui->buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonsAction(QAbstractButton*)));
    connect(ui->showTextCB, SIGNAL(toggled(bool)), ui->textL, SLOT(setEnabled(bool)));
    connect(ui->showTextCB, SIGNAL(toggled(bool)), ui->textLE, SLOT(setEnabled(bool)));

    loadSettings();

    connect(ui->showTextCB, SIGNAL(toggled(bool)), this, SLOT(showTextChanged(bool)));
    connect(ui->textLE, SIGNAL(textEdited(QString)), this, SLOT(textButtonChanged(QString)));
    connect(ui->chooseMenuFilePB, SIGNAL(clicked()), this, SLOT(chooseMenuFile()));
}

GeuzenMenuConfig::~GeuzenMenuConfig()
{
    delete ui;
}

void GeuzenMenuConfig::loadSettings()
{
    ui->showTextCB->setChecked(mSettings.value("showText", false).toBool());
    ui->textLE->setText(mSettings.value("text", "").toString());

    QString menuFile = mSettings.value("menu_file", "").toString();
    if (menuFile.isEmpty())
    {
        menuFile = XdgMenu::getMenuFileName();
    }
    ui->menuFilePathLE->setText(menuFile);
}

void GeuzenMenuConfig::textButtonChanged(QString value)
{
    mSettings.setValue("text", value);
}

void GeuzenMenuConfig::showTextChanged(bool value)
{
    mSettings.setValue("showText", value);
}

void GeuzenMenuConfig::chooseMenuFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose menu file"), "~", tr("Menu files (*.menu)"));
    if (!path.isEmpty())
    {
        ui->menuFilePathLE->setText(path);
        mSettings.setValue("menu_file", path);
    }
}

void GeuzenMenuConfig::dialogButtonsAction(QAbstractButton *btn)
{
    if (ui->buttons->buttonRole(btn) == QDialogButtonBox::ResetRole)
    {
        mOldSettings.loadToSettings();
        loadSettings();
    }
    else
    {
        close();
    }
}
