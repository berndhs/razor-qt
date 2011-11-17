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
  daroles[Type_IconName] = "iconName";
  daroles[Type_Name] = "itemTitle";
  daroles[Type_IsSubmenu] = "isMenu";
  daroles[Type_ItemTag] = "itemTag";
  daroles[Type_EntryKind] = "itemKind";
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
  case Type_IconName:
    retVar = item.iconName;
    break;
  case Type_Name:
    retVar = item.title;
    break;
  case Type_IsSubmenu:
    retVar = (item.tipo == Entry_Menu);
    break;
  case Type_ItemTag:
    retVar = item.tag;
    break;
  case Type_EntryKind:
    retVar = item.tipo;
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
  beginInsertRows (QModelIndex(), items.count(), items.count());
  items.append (Entry (title, desktop, Entry_Menu, menuTag));
  endInsertRows ();
  std::cerr << __PRETTY_FUNCTION__
            << " item " << items.count()
            << " " << title.toStdString()
            << " " << desktop.toStdString ()
            << std::endl;
}

void
MenuModel::addAppLink (const QString & title,
                       const QString & desktop,
                       int       appTag,
                       const QString   & iconName)
{
  beginInsertRows (QModelIndex(), items.count(), items.count());
  items.append (Entry (title, desktop, Entry_Application, appTag, iconName));
  endInsertRows ();
  std::cerr << __PRETTY_FUNCTION__
            << " item " << items.count()
            << " " << title.toStdString()
            << " " << desktop.toStdString ()
            << std::endl;
}

void
MenuModel::addNavigate (const QString & title,
                        int tag)
{
  beginInsertRows (QModelIndex(), items.count(), items.count());
  items.append (Entry (title, "", Entry_Navigate, tag));
  endInsertRows ();
  std::cerr << __PRETTY_FUNCTION__
            << " item " << items.count()
            << " " << title.toStdString()
            << " navigate " << tag
            << std::endl;
}

void
MenuModel::fakeReset ()
{
  beginResetModel ();
  endResetModel ();
}

} // namespace
