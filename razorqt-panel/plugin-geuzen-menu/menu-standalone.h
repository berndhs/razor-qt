#ifndef GEUZEN_MENU_STANDALONE_H
#define GEUZEN_MENU_STANDALONE_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include "menu-view.h"
#include <qtxdg/xdgmenu.h>


class PowerManager;

namespace geuzen
{
class StandaloneMenu : public QWidget
{
Q_OBJECT
public:

  StandaloneMenu (QWidget *parent=0);

  void run ();

private slots:

  void quit ();
  void showMenu ();
  void hideMenu ();
  void buildMenu ();

signals:

  void exiting ();

private:

  MenuView     * menuView;
  QPushButton  * mainButton;
  QString        mLogDir;
  QString        menuFile;
  XdgMenu        xdgMenu;
  PowerManager*  powerManager;
  

};

} // namespace

#endif
