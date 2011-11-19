#include "menu-view.h"
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <QtDeclarative/QDeclarativeEngine>

namespace geuzen
{
MenuView::MenuView (const XdgMenu & xdgMenu,
                    const QString & title,
                    QWidget *parent)
  :QDeclarativeView (parent),
   topModel (0),
   nextSubTag (0),
   nextAppTag (0),
   menuImage ("menuimage"),
   menuImageUrl (QString("image://menuicons/") + menuImage)
{
  setStyleSheet ("background:transparent;");

  topModel = new MenuModel (this);
  topModelTag = nextSubTag;
  topModel->addNavigate (QString ("0-0-0"),-1);
  modelTagStack.prepend (topModelTag);
  subMenus[topModelTag] = topModel;
  nextSubTag++;
  readModel (topModel, xdgMenu);
  topModel->setTitle (tr("Applications"));
  setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
  //setAttribute (Qt::WA_NoSystemBackground);
  //setAttribute (Qt::WA_TranslucentBackground);
  engine()->addImageProvider (QLatin1String("menuicons"),&imagePro);
  imagePro.addIcon (menuImage,QIcon (":/img/lines.png"));
}

void
MenuView::selected (int kind, int tag)
{
  std::cerr << __PRETTY_FUNCTION__ << " kind " << kind << " tag " << tag << std::endl;
 
  switch (kind) {
  case Entry_Menu:
    switchMenu (tag);
    break;
  case Entry_Application:
    startApplication (tag);
    break;
  case Entry_Navigate:
    navigate (tag);
    break;
  default:
    std::cerr << "      bad kind " << kind;
    break;
  }
}

void
MenuView::switchMenu (int menuTag)
{
  std::cerr << __PRETTY_FUNCTION__ << " tag " << menuTag << std::endl;
  if (subMenus.contains (menuTag)) {
    MenuModel * subModel = subMenus[menuTag];
    if (subModel) {
      context->setContextProperty ("cppMenuModel",subModel);
      subModel->fakeReset ();
      if (qmlRoot) {
        qmlRoot->setProperty ("menuTitle",subModel->title());
      }
    }
  }
}

void
MenuView::startApplication (int appTag)
{
  std::cerr << __PRETTY_FUNCTION__ << " tag " << appTag<< std::endl;
  if (apps.contains (appTag)) {
     std::cerr << "   start app " 
               << apps[appTag].fileName().toStdString() << std::endl;
     apps[appTag].startDetached ();
  }
  navigate (-1);
}

void
MenuView::navigate (int naviTag)
{
  std::cerr << __PRETTY_FUNCTION__ << " tag " << naviTag<< std::endl;
  if (naviTag < 0) {
    switchMenu (topModelTag);
    hide ();
  } else {
    switchMenu (naviTag);
  }
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
  qDebug () << __PRETTY_FUNCTION__ << pos << " size " << size();
  
  move (pos);
  show ();
  qDebug () << __PRETTY_FUNCTION__ << isVisible ();
}

void
MenuView::init (ViewType viewType)
{
  qDebug () << __PRETTY_FUNCTION__ << viewType;
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
    connect (qmlRoot, SIGNAL (selected (int, int)),
             this, SLOT (selected (int, int)));
    qmlRoot->setProperty ("menuTitle",topModel->title());
  }
  //hide ();
  qDebug () << __PRETTY_FUNCTION__ << " source is " << source();
}

void
MenuView::cancel ()
{
  hide ();
}

void
MenuView::parseDom (MenuModel * parseModel, const QDomElement & root)
{
  QDomElement  elt;
  for (elt = root.firstChildElement ();
       !elt.isNull();
       elt = elt.nextSiblingElement ()) {
    std::cerr << " tag " << elt.tagName().toStdString() << std::endl;
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
  std::cerr << "  submenu title " << title.toStdString() << std::endl;
  QString desktopFile = root.attribute ("desktopFile");
  MenuModel * subModel = new MenuModel (this);
  subModel->setTitle (title);
  int subTag = nextSubTag;
  nextSubTag++;
  int previousTag = modelTagStack.first();
  subMenus[subTag] = subModel;
  parseModel->addSubmenu (title, desktopFile, subTag, menuImageUrl);
  subModel->addNavigate (QString ("<<"),topModelTag);
  subModel->addNavigate (QString ("<"),previousTag);
  modelTagStack.prepend (subTag);
  parseDom (subModel, root);
  modelTagStack.removeFirst ();
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
  QString desktopPath = elt.attribute ("desktopFile");
  apps[nextAppTag] = XdgDesktopFile (desktopPath, this);
  XdgDesktopFile & desk (apps[nextAppTag]);
  QString imageName (QString("appimg%1").arg(nextAppTag));
  QString imageUrl (QString ("image://menuicons/%1").arg(imageName));
  imagePro.addIcon (imageName, desk.icon());
  parseModel->addAppLink (desk.name(), desktopPath, 
                          nextAppTag, imageUrl);
  nextAppTag++;
}


} // namespace