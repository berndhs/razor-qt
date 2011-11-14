#include "menu-view.h"
#include <QDebug>
#include <QMessageBox>
#include <iostream>

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
  setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
}

void
MenuView::reload (const XdgMenu & xdgMenu)
{
  qDebug () << __PRETTY_FUNCTION__;
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
  qDebug () << __PRETTY_FUNCTION__;
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
  show ();
  context = rootContext();
  context->setContextProperty ("cppMenuModel",topModel);
  if (!qmlName.isEmpty()) {
    setSource (QUrl (QString("qrc:///qml/%1").arg (qmlName)));
  }
  qmlRoot = qobject_cast <QDeclarativeItem*> (rootObject());
  if (qmlRoot) {
    connect (qmlRoot, SIGNAL (cancelled()), this, SLOT (cancel()));
  }
  hide ();
}

void
MenuView::cancel ()
{
  hide ();
}

void
MenuView::parseDom (MenuModel * parseModel, const QDomElement & root)
{
  std::cout << __PRETTY_FUNCTION__ << parseModel << std::endl;
  QDomElement  elt;
  for (elt = root.firstChildElement ();
       !elt.isNull();
       elt = elt.nextSiblingElement ()) {
    std::cout << " tag " << elt.tagName().toStdString() << std::endl;
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
  std::cout << "  submenu title " << title.toStdString() << std::endl;
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
  std::cout << "  application title " << title.toStdString() << std::endl;
  QString desktopFile = elt.attribute ("desktopFile");
  apps[nextAppTag] = desktopFile;
  parseModel->addAppLink (title, desktopFile, nextAppTag);
  nextAppTag++;
}


} // namespace