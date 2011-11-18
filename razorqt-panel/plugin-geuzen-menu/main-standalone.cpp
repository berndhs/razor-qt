#include <QApplication>

#include "menu-standalone.h"

using namespace geuzen;

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  StandaloneMenu  menu;
  menu.setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
  menu.move (0,0);
  menu.run();

  QObject::connect (&menu, SIGNAL(exiting()), &app, SLOT(quit()));

  menu.run ();

  return app.exec ();
  
}
