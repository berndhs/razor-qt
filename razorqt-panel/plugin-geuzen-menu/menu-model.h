#ifndef GEUZEN_MENU_MODEL_H
#define GEUZEN_MENU_MODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>

namespace geuzen
{

class MenuModel : public QAbstractListModel
{
Q_OBJECT
public:
  MenuModel (QObject * parent=0);

  int rowCount (const QModelIndex & parent = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  void addSubmenu (const QString & title,
                   const QString & desktop,
                         int       menuTag);
  void addAppLink (const QString & title,
                   const QString & desktop,
                         int       appTag);

private:

  enum DataType {
    Type_ImageFile = Qt::UserRole +1,
    Type_Name      = Qt::UserRole +2,
    Type_IsSubmenu = Qt::UserRole +3,
    Type_MenuTag   = Qt::UserRole +4,
    Type_LeafTag   = Qt::UserRole +5
  };
  enum EntryKind {
    Entry_None = 0,
    Entry_Menu = 1,
    Entry_Application = 2
  };

  struct Entry {
    Entry (QString ti, QString d, EntryKind ty, int tg)
      :title (ti), desktop (d), tipo (ty), tag (tg)
    {}
    QString    title;
    QString    desktop;
    EntryKind  tipo;
    int        tag;
  };

  QList <Entry>  items;
}; 

} // namespace

#endif
