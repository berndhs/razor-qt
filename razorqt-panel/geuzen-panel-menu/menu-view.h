#ifndef GEUZEN_MENU_VIEW_H
#define GEUZEN_MENU_VIEW_H

#include <QDeclarativeView>
#include <QDomElement>
#include <QDomDocument>

#include "menu-model.h"

namespace geuzen
{
class MenuView : public QDeclarativeView
{
Q_OBJECT
public:

  MenuView (const XdgMenu& xdgMenu,
            const QString & title = QString(),
                  QWidget *parent=0);

private slots:

  void goToTop ();
  void goUp ();
  void goDown (int menuTag);

private:

  typedef QMap <int, MenuModel*>  ModelMap;
  typedef QMap <int, QString>     AppMap;

  void readModel (const XdgMenu & xdgMenu);
  void parseDom (const QDomElement & root);
  void startSubMenu (const QDomElement & root);
  void insertAppLink (const QDomElement & elt);


  MenuModel    *topModel;
  MenuModel    *currentModel;
  MenuModel    *parseCurrentModel;
  int           nextSubTag;
  int           nextAppTag;
  ModelMap      subMenus;
  AppMap        appMap;
  
};

} // namespace


#endif
