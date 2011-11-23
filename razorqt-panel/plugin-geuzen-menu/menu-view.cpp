#include "menu-view.h"
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <QtDeclarative/QDeclarativeEngine>
#include <qtxdg/xdgicon.h>

namespace geuzen
{
MenuView::MenuView (const XdgMenu & xdgMenu,
                    const QString & title,
                    QWidget *parent)
  :QDeclarativeView (parent),
   topModel (0),
   nextSubTag (10001),
   nextAppTag (20001),
   nextActionTag (40001),
   menuImage ("menuimage"),
   menuImageUrl (QString("image://menuicons/") + menuImage),
   topIcon ("topicion"),
   topIconUrl (QString("image://menuicons/") + topIcon),
   backIcon ("backicon"),
   backIconUrl (QString("image://menuicons/") + backIcon)
{
  setStyleSheet ("background:transparent;");

  topModel = new MenuModel (this);
  topModel->setTitle (title);
  topModelTag = nextSubTag;
  topModel->addItem (QString ("0-0-0"), Entry_Navigate,-1, QString());
  modelTagStack.prepend (topModelTag);
  subMenus[topModelTag] = topModel;
  nextSubTag++;
  setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
  //setAttribute (Qt::WA_NoSystemBackground);
  //setAttribute (Qt::WA_TranslucentBackground);
  engine()->addImageProvider (QLatin1String("menuicons"),&imagePro);
  imagePro.addIcon (menuImage,QIcon (":/img/lines.png"));
  imagePro.addIcon (topIcon,XdgIcon::fromTheme ("start-here"));
  imagePro.addIcon (backIcon, XdgIcon::fromTheme ("go-previous",
                            QIcon (":/img/less.png")));
  readModel (topModel, xdgMenu);
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
  case Entry_Action:
    triggerAction (tag);
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
  navigate ();
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
MenuView::triggerAction (int actionTag)
{
  std::cerr << __PRETTY_FUNCTION__ << " tag " << actionTag 
            << std::endl;
  if (actions.contains (actionTag)) {
    QAction * act = actions[actionTag];
    std::cerr << __PRETTY_FUNCTION__ << " found action " << act 
              << std::endl;
    if (act) {
      std::cerr << __PRETTY_FUNCTION__ << " action " << act->text().toStdString()
                << std::endl;
      act->trigger();
      navigate ();
    }
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
MenuView::appendMenu (const XdgMenu & xdgMenu)
{
  if (topModel) {
    readModel (topModel, xdgMenu);
  }
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
  std::cerr << __PRETTY_FUNCTION__  << std::endl
            << "     title [" << root.attribute("title").toStdString()
            << "]" << std::endl
            << "      name [" << root.attribute("name").toStdString()
            << "]" << std::endl
           ;
  if (root.attribute("title").isEmpty()) {
    title = root.attribute ("name");
  } else {
    title = root.attribute ("title");
  }
  QString desktopFile = root.attribute ("desktopFile");
  MenuModel * subModel = new MenuModel (this);
  subModel->setTitle (title);
  int subTag = nextSubTag;
  nextSubTag++;
  int previousTag = modelTagStack.first();
  subMenus[subTag] = subModel;
  parseModel->addItem (title, Entry_Menu, subTag, 
                        submenuIconUrl(title, subTag, menuImageUrl));
  subModel->addItem (tr ("Top"),Entry_Navigate, topModelTag, 
                          topIconUrl);
  subModel->addItem (tr ("Back"),Entry_Navigate,previousTag,
                           backIconUrl);
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
  parseModel->addItem (desk.name(), Entry_Application, 
                          nextAppTag, imageUrl);
  nextAppTag++;
}

void
MenuView::appendSubmenuActions (const QString & title,
                             const QIcon & icon,
                             QList<QAction *>  acts)
{
  std::cerr << __PRETTY_FUNCTION__ << title.toStdString() << std::endl;
  MenuModel * subModel = new MenuModel (this);
  subModel->setTitle (title);
  int subTag = nextSubTag;
  nextSubTag++;
  subMenus[subTag] = subModel;
  QString imageName (QString("menuimg%1").arg(subTag));
  QString imageUrl (QString ("image://menuicons/%1").arg(imageName));
  imagePro.addIcon (imageName, icon);
  topModel->addItem (title, Entry_Menu, subTag, imageUrl);
  subModel->addItem (QString ("<<"),Entry_Navigate, topModelTag, 
                          topIconUrl);
  for (int a=0; a<acts.count(); a++) {
    int tag = nextActionTag;
    nextActionTag ++;
    QAction *act = acts.at(a);
    actions[tag] = act;
    imageName = QString ("actionimg%1").arg(tag);
    imageUrl = QString ("image://menuicons/%1").arg(imageName);
    imagePro.addIcon (imageName, icon);
    subModel->addItem (act->text(), Entry_Action, tag, imageUrl);
    std::cerr << __PRETTY_FUNCTION__ << " act " << act
              << " title " << act->text().toStdString()
              << " imgUrl " << imageUrl.toStdString()
              << " tag " << tag
              << std::endl;
  }
}

QString
MenuView::submenuIconUrl (const QString & title,
                                int       tag,
                          const QString & defaultUrl)
{
  QString flatName (title.toLower());
  QStringList prefixes;
  prefixes << "applications" << "preferences" << "system";
  for (int p=0; p<prefixes.count(); p++) {
    QString tryName (prefixes.at(p) + "-" + flatName);
    QIcon tryIcon (XdgIcon::fromTheme (tryName));
    if (!tryIcon.isNull ()) {
      QString imageName (QString("submenuimg%1").arg(tag));
      QString imageUrl (QString ("image://menuicons/%1").arg(imageName));
      imagePro.addIcon (imageName, tryIcon);
      return imageUrl;
    }
  }
  return defaultUrl;
}

} // namespace