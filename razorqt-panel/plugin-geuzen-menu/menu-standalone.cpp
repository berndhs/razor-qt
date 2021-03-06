#include <QIcon>
#include <QCursor>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include "menu-standalone.h"
#include <razorqt/powermanager.h>
#include <qtxdg/xdgicon.h>

namespace geuzen
{

StandaloneMenu::StandaloneMenu (QWidget *parent)
  :QWidget(parent),
   menuView (0),
   mainButton (0),
   menuFile ("/etc/xdg/menus/razor-applications.menu")
{
  setObjectName ("StandaloneMenu");
  setWindowIcon (QIcon(":/geuzenmenu.png"));
  mainButton = new QPushButton (QIcon(":/geuzenmenu.png"),
                                tr("Applications"),
                                this);
  mainButton->resize (180,48);
  powerManager = new PowerManager(this);
  buildMenu ();
  connect (mainButton,SIGNAL (clicked()), this, SLOT(showMenu()));
  QTimer::singleShot (2, this, SLOT(hideMenu ()));
}

void
StandaloneMenu::run ()
{
  show ();
}

void
StandaloneMenu::showMenu ()
{
  qDebug () << __PRETTY_FUNCTION__ << " outdated ? " 
            << xdgMenu.isOutDated();
  QPoint pos (QCursor::pos());
  if (xdgMenu.isOutDated()) {
    buildMenu();
  }
  if (!menuView) {
    return;
  }
  menuView->resize (600,400);
  menuView->exec (pos);
}

void
StandaloneMenu::hideMenu ()
{
  if (menuView) {
    menuView->hide();
  }
}

void
StandaloneMenu::buildMenu ()
{
  xdgMenu.setEnvironments ("X-RAZOR");
  xdgMenu.setLogDir(mLogDir);
  bool res = xdgMenu.read(menuFile);
  if (res) {
    if (!menuView) {
      menuView = new MenuView (xdgMenu, "", 0);
      menuView->setResizeMode (QDeclarativeView::SizeRootObjectToView);
      menuView->init (MenuView::GridView);
      menuView->setObjectName("TopLevelMainMenu");
    } else {
      menuView->reload (xdgMenu);
    }
  } else {
    QMessageBox::warning(this, "Parse error", xdgMenu.errorString());
    return;
  }
  if (menuView) {
    menuView->appendSubmenuActions (
             tr("Leave"), 
             XdgIcon::fromTheme ("system-log-out"),
             powerManager->availableActions());
  }
}

void
StandaloneMenu::quit ()
{
  emit exiting();
}

} // namespace
