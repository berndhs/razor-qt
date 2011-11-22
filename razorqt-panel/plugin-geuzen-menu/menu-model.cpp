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

QString
MenuModel::title() const
{
  return theTitle;
}

void
MenuModel::setTitle (const QString & title)
{
  theTitle = title;
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
MenuModel::addItem (const QString & title,
                       MenuEntryKind kind,
                       int       tag,
                       const QString & iconName)
{
  beginInsertRows (QModelIndex(), items.count(), items.count());
  items.append (Entry (title, kind, tag, iconName));
  endInsertRows ();
}


void
MenuModel::fakeReset ()
{
  beginResetModel ();
  endResetModel ();
}

} // namespace
