/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Bernd Stramm
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.ru>
 *   Bernd Stramm <bernd.stramm@@gmail.com>
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


#ifndef GEUZEN_MAINMENU_H
#define GEUZEN_MAINMENU_H

#include "../panel/razorpanelplugin.h"
#include <qtxdg/xdgmenu.h>

#include <QPushButton>
#include <QDomElement>
#include <QAction>

#include "menustyle.h"
#include "menu-view.h"

class QMenu;
class RazorBar;
class PowerManager;
class ScreenSaver;

namespace geuzen
{

class GeuzenMainMenu : public RazorPanelPlugin
{
  Q_OBJECT
public:
  GeuzenMainMenu(const RazorPanelPluginStartInfo* startInfo, QWidget* parent = 0);
  ~GeuzenMainMenu();

  virtual RazorPanelPlugin::Flags flags() const {
    return HaveConfigDialog ;
  }

private:
  QPushButton mButton;
  QString mLogDir;
  QString mMenuFile;
  MenuStyle mTopMenuStyle;
  MenuStyle mMenuStyle;
  PowerManager* mPowerManager;
  ScreenSaver* mScreenSaver;
  MenuView  * mMenu;
  XdgMenu mXdgMenu;
  void buildMenu();

protected slots:
  virtual void showConfigureDialog();
  virtual void settigsChanged();

private slots:
  void showMenu();
};

} // namespace

EXPORT_RAZOR_PANEL_PLUGIN_H

#endif
