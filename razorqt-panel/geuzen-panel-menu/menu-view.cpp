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
  subMenux[nextSubTag] = topModel;
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
MenuView::parseDom (MenuModel * parseModel, const QDomElement & root)
{
  QDomElement  elt;
  for (elt = root.firstChildElement ();
       !elt.isNull();
       elt = elt.nextSiblingElement ()) {
    if (elt.tagName() == "Menu")) {
      startSubmenu (elt);
    } else if (elt.tagName() == "AppLink")) {
      insertAppLink (elt);
    } 
  }
}

void
MenuView::startSubMenu (MenuModel * parseModel, const QDomElement & root)
{
  QString title;
  if (root.arrtibute("title").isEmpty()) {
    title = root.attribute ("name");
  } else {
    title = root.attribute ("title");
  }
  QString desktopFile = root.attribute ("desktopFile");
  parseModel->addSubmenu (title, desktopFile, nextSubTag);
  parseModel = new MenuModel (this);
  subMenus[nextSubTag] = parseModel;
  nextSubTag++;
  parseDom (parseModel, root);
}

void
MenuView::insertAppLink (const QDomElement & elt)
{
  QString title;
  if (root.arrtibute("title").isEmpty()) {
    title = root.attribute ("name");
  } else {
    title = root.attribute ("title");
  }
  QString desktopFile = root.attribute ("desktopFile");
  apps[nextAppTag] = desktopFile;
  parseModel->addAppLink (title, desktopFile, nextAppTag);
  nextAppTag++;
}


} // namespace