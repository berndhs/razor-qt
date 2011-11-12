#ifndef GEUZEN_MENU_VIEW_H
#define GEUZEN_MENU_VIEW_H

#include <QtDeclarative/QDeclarativeView>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>

#include <qtxdg/xdgmenu.h>

#include "menu-model.h"

namespace geuzen
{
class MenuView : public QDeclarativeView
{
Q_OBJECT
public:

  enum ViewType {
    GridView = 1,
    ListView = 2
  };

  MenuView (const XdgMenu& xdgMenu,
            const QString & title = QString(),
                  QWidget *parent=0);

  void reload (const XdgMenu & xdgMenu);

public slots:

  void init (ViewType viewType);

  void exec (const QPoint & pos);

private slots:

  void goToTop ();
  void goUp ();
  void goDown (int menuTag);

private:

  typedef QMap <int, MenuModel*>  ModelMap;
  typedef QMap <int, QString>     AppMap;

  void readModel (MenuModel * parseMode, const XdgMenu & xdgMenu);
  void parseDom (MenuModel * parseMode, const QDomElement & root);
  void startSubMenu (MenuModel * parseMode, const QDomElement & root);
  void insertAppLink (MenuModel * parseMode, const QDomElement & elt);


  MenuModel    *topModel;
  MenuModel    *currentModel;
  int           nextSubTag;
  int           nextAppTag;
  ModelMap      subMenus;
  AppMap        apps;
  
};

} // namespace


#endif
