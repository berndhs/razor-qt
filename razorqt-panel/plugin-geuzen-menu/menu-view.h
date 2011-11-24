#ifndef GEUZEN_MENU_VIEW_H
#define GEUZEN_MENU_VIEW_H

#include <QtDeclarative/QDeclarativeView>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeContext>
#include <QList>
#include <QAction>
#include <QEvent>


#include <qtxdg/xdgmenu.h>
#include <qtxdg/xdgdesktopfile.h>
#include "geuzen-imagepro.h"
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
  void appendMenu (const XdgMenu & xdgMenu);
  void appendSubmenuActions (const QString & title,
                             const QIcon & icon,
                             QList<QAction*>  actions);

public slots:

  void init (ViewType viewType);

  void exec (const QPoint & pos);
  void cancel ();

protected:

  void leaveEvent (QEvent * evt);

private slots:

  void selected (int kind, int tag);

private:

  typedef QMap <int, MenuModel*>         ModelMap;
  typedef QMap <int, XdgDesktopFile>     AppMap;
  typedef QMap <int, QAction*>           ActionMap;

  /** functions to load/parse the model */

  void readModel (MenuModel * parseMode, const XdgMenu & xdgMenu);
  void parseDom (MenuModel * parseMode, const QDomElement & root);
  void startSubMenu (MenuModel * parseMode, const QDomElement & root);
  void insertAppLink (MenuModel * parseMode, const QDomElement & elt);
  QString submenuIconUrl (const QString & title,
                            int tag,
                        const QString & defaultUrl);

  /** functions to navigate through the menu */

  void switchMenu (int menuTag);
  void startApplication (int appTag);
  void navigate (int naviTag = -1);
  void triggerAction (int actionTag);


  MenuModel            *topModel;
  int                   topModelTag;
  QList<int>            modelTagStack;
  int                   nextSubTag;
  int                   nextAppTag;
  int                   nextActionTag;
  QString               menuImage;
  QString               menuImageUrl;
  QString               topIcon;
  QString               topIconUrl;
  QString               backIcon;
  QString               backIconUrl;
  ModelMap              subMenus;
  AppMap                apps;
  ActionMap             actions;
  QDeclarativeItem    * qmlRoot;
  QDeclarativeContext * context;
  ImagePro              imagePro;
};

} // namespace


#endif
