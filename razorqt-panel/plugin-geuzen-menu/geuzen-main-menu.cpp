/*
 *
 * Copyright: 2011 Bernd Stramm
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
 *
 */


#include "geuzen-main-menu.h"
#include "geuzen-menu-config.h"
#include <QDebug>
#include <QtGui/QMenu>
#include <qtxdg/xdgdesktopfile.h>
#include <qtxdg/xmlhelper.h>
#include <QSettings>
#include <QFileInfo>
#include <QAction>
#include <QtGui/QMessageBox>
#include <razorqt/powermanager.h>
#include <razorqt/screensaver.h>

#include <qtxdg/xdgicon.h>
#include <qtxdg/xdgdesktopfile.h>
#include <qtxdg/xdgmenuwidget.h>

#include <QStack>

#include <QCursor>
EXPORT_RAZOR_PANEL_PLUGIN_CPP(geuzen::GeuzenMainMenu)

namespace geuzen
{
/************************************************

 ************************************************/
GeuzenMainMenu::GeuzenMainMenu(const RazorPanelPluginStartInfo* startInfo,
                               QWidget* parent):
  RazorPanelPlugin(startInfo, parent),
  mMenu(0)
{
  setObjectName("MainMenu");

  mButton.setIcon (QIcon(":/geuzenmenu.png"));
  addWidget(&mButton);
  connect(&mButton, SIGNAL(clicked()), this, SLOT(showMenu()));
  mPowerManager = new PowerManager(this);
  mPowerManager->setParentWidget(panel());

  mScreenSaver = new ScreenSaver(this);

  settigsChanged();

  QSizePolicy sp = mButton.sizePolicy();
  sp.setVerticalPolicy(QSizePolicy::Minimum);
  mButton.setSizePolicy(sp);

}


/************************************************

 ************************************************/
GeuzenMainMenu::~GeuzenMainMenu()
{
}


/************************************************

 ************************************************/
void GeuzenMainMenu::showMenu()
{
  if (mXdgMenu.isOutDated())
    buildMenu();

  if (!mMenu)
    return;

  int x, y;

  switch (panel()->position()) {
  case RazorPanel::PositionTop:
    x = mButton.mapToGlobal(QPoint(0, 0)).x();
    y = panel()->mapToGlobal(QPoint(0, panel()->sizeHint().height())).y();
    break;

  case RazorPanel::PositionBottom:
    x = mButton.mapToGlobal(QPoint(0, 0)).x();
    y = panel()->mapToGlobal(QPoint(0, 0)).y() - mMenu->sizeHint().height();
    break;

  case RazorPanel::PositionLeft:
    x = panel()->mapToGlobal(QPoint(panel()->sizeHint().width(), 0)).x();
    y = mButton.mapToGlobal(QPoint(0, 0)).y();
    break;

  case RazorPanel::PositionRight:
    x = panel()->mapToGlobal(QPoint(0, 0)).x() - mMenu->sizeHint().width();
    y = mButton.mapToGlobal(QPoint(0, 0)).y();
    break;

  }

  QPoint pos(x, y);
  mMenu->exec(pos);
}


/************************************************

 ************************************************/
void GeuzenMainMenu::settigsChanged()
{
  if (settings().value("showText", false).toBool() == false) {
    mButton.setText(NULL);
  } else {
    mButton.setText(settings().value("text", "Start").toString());
  }
  mLogDir = settings().value("log_dir", "").toString();
  mTopMenuStyle.setIconSize(settings().value("top_icon_size", 16).toInt());

  mMenuFile = settings().value("menu_file", "").toString();
  if (mMenuFile.isEmpty())
    mMenuFile = XdgMenu::getMenuFileName();
}


/************************************************

 ************************************************/
void GeuzenMainMenu::buildMenu()
{
  mXdgMenu.setEnvironments("X-RAZOR");
  mXdgMenu.setLogDir(mLogDir);

  bool res = mXdgMenu.read(mMenuFile);
  if (res) {
    if (!mMenu) {
      mMenu = new MenuView (mXdgMenu, "", this);
      mMenu->init (MenuView::GridView);
      mMenu->setObjectName("TopLevelMainMenu");
      mMenu->setStyle(&mTopMenuStyle);
    } else {
      mMenu->reload (mXdgMenu);
    }
  } else {
    QMessageBox::warning(this, "Parse error", mXdgMenu.errorString());
    return;
  }

#if 0
  QMenu* leaveMenu = mMenu->addMenu(XdgIcon::fromTheme("system-shutdown"), tr("Leave"));
  leaveMenu->addActions(mPowerManager->availableActions());

  mMenu->addActions(mScreenSaver->availableActions());
#endif
}

void GeuzenMainMenu::showConfigureDialog()
{
  GeuzenMenuConfig *confWindow =
    this->findChild<GeuzenMenuConfig*>("MainMenuConfigWindow");

  if (!confWindow) {
    confWindow = new GeuzenMenuConfig(settings(), this);
  }

  confWindow->show();
  confWindow->raise();
  confWindow->activateWindow();
}

} // namespace

