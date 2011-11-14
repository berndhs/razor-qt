#include "menu-model.h"

#include <QHash>
#include <QByteArray>
#include <QDebug>
#include <iostream>

namespace geuzen
{

MenuModel::MenuModel (QObject *parent)
  :QAbstractListModel (parent)
{
  QHash <int, QByteArray> daroles;
  daroles[Type_ImageFile] = "imagename";
  daroles[Type_Name] = "title";
  daroles[Type_IsSubmenu] = "isMenu";
  daroles[Type_MenuTag] = "menuTag";
  daroles[Type_LeafTag] = "appTag";
  setRoleNames (daroles);
}

int
MenuModel::rowCount (const QModelIndex & parent) const
{
  Q_UNUSED (parent);
  return items.count ();
}

QVariant
MenuModel::data (const QModelIndex & index, int role) const
{
  int row (index.row());
  QVariant retVar;
  if (row < 0 || row >= items.count()) {
    return retVar;
  }
  Entry  item = items.at(row);
  switch (role) {
  case Type_Name:
    retVar = item.title;
    break;
  case Type_IsSubmenu:
    retVar = (item.tipo == Entry_Menu);
    break;
  case Type_MenuTag:
    retVar = (item.tipo == Entry_Menu ? item.tag : -1);
    break;
  case Type_LeafTag:
    retVar = (item.tipo == Entry_Application ? item.tag : -1);
    break;
  default:
    break;
  }
  return retVar;
}

void
MenuModel::addSubmenu (const QString & title,
                       const QString & desktop,
                       int       menuTag)
{
  items.append (Entry (title, desktop, Entry_Menu, menuTag));
  std::cout << __PRETTY_FUNCTION__
            << " item " << items.count()
            << " " << title.toStdString()
            << " " << desktop.toStdString ()
            << std::endl;
}

void
MenuModel::addAppLink (const QString & title,
                       const QString & desktop,
                       int       appTag)
{
  items.append (Entry (title, desktop, Entry_Application, appTag));
  std::cout << __PRETTY_FUNCTION__
            << " item " << items.count()
            << " " << title.toStdString()
            << " " << desktop.toStdString ()
            << std::endl;
}

} // namespace
