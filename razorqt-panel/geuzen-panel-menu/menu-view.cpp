#include "menu-view.h"

namespace geuzen
{
MenuView::MenuView (const XdgMenu & xdgMenu,
                    const QString & title,
                          QWidget *parent)
  :QDeclarativeView (parent),
   topModel (0),
   currentModel (0),
   nextSubTag (0),
   nextAppTag (0)
{
  topModel = new MenuModel (this);
  subMenus[nextSubTag] = topModel;
  nextSubTag++;
  readModel (topModel, xdgMenu);
}

void
MenuView::reload (const XdgMenu & xdgMenu)
{
  subMenus.clear ();
  apps.clear ();
  if (topModel) {
    topModel->deleteLater ();
  }
  topModel = new MenuModel (this);
  subMenus[nextSubTag] = topModel;
  nextSubTag++;
  readModel (topModel, xdgMenu);
}

void
MenuView::readModel (MenuModel * parseModel, const XdgMenu & xdgMenu)
{
  const QDomDocument doc (xdgMenu.xml());
  QDomElement root = doc.firstChildElement ("");
  parseDom (parseModel, root);
} 

void
MenuView::exec (const QPoint & pos)
{
  move (pos);
  show ();
}

void
MenuView::init (ViewType viewType)
{
  QString qmlName;
  switch (viewType) {
  case GridView:
    qmlName = "Gridmenu.qml";
    break;
  case ListView:
    qmlName = "Listmenu.qml";
    break;
  default:
    break;
  }
  if (!qmlName.isEmpty()) {
    setSource (QUrl (QString("qrc:/qml/%1").arg (qmlName)));
  }
  hide ();
}

void
MenuView::parseDom (MenuModel * parseModel, const QDomElement & root)
{
  QDomElement  elt;
  for (elt = root.firstChildElement ();
       !elt.isNull();
       elt = elt.nextSiblingElement ()) {
    if (elt.tagName() == "Menu") {
      startSubMenu (parseModel, elt);
    } else if (elt.tagName() == "AppLink") {
      insertAppLink (parseModel, elt);
    } 
  }
}

void
MenuView::startSubMenu (MenuModel * parseModel, const QDomElement & root)
{
  QString title;
  if (root.attribute("title").isEmpty()) {
    title = root.attribute ("name");
  } else {
    title = root.attribute ("title");
  }
  QString desktopFile = root.attribute ("desktopFile");
  MenuModel * subModel = new MenuModel (this);
  subMenus[nextSubTag] = subModel;
  parseModel->addSubmenu (title, desktopFile, nextSubTag);
  nextSubTag++;
  parseDom (subModel, root);
}

void
MenuView::insertAppLink (MenuModel * parseModel, const QDomElement & elt)
{
  QString title;
  if (elt.attribute("title").isEmpty()) {
    title = elt.attribute ("name");
  } else {
    title = elt.attribute ("title");
  }
  QString desktopFile = elt.attribute ("desktopFile");
  apps[nextAppTag] = desktopFile;
  parseModel->addAppLink (title, desktopFile, nextAppTag);
  nextAppTag++;
}


} // namespace