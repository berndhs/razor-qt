#ifndef GEUZEN_MENU_MODEL_H
#define GEUZEN_MENU_MODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>

namespace geuzen
{

enum MenuEntryKind {
  Entry_None = 0,
  Entry_Menu = 1,
  Entry_Application = 2,
  Entry_Navigate = 3,
  Entry_Action = 4
};

class MenuModel : public QAbstractListModel
{
  Q_OBJECT
public:


  MenuModel (QObject * parent=0);
  ~MenuModel ();

  int rowCount (const QModelIndex & parent = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  void addItem (const QString & title,
                   MenuEntryKind kind,
                   int           tag,
                   const QString & iconName);

  QString title () const;
  void    setTitle (const QString & title);

  void clearModel ();
  void forceReset ();

private:

  enum DataType {
    Type_IconName  = Qt::UserRole +1,
    Type_Name      = Qt::UserRole +2,
    Type_IsSubmenu = Qt::UserRole +3,
    Type_ItemTag   = Qt::UserRole +4,
    Type_EntryKind = Qt::UserRole +5
  };

  struct Entry {
    Entry (const QString & ti = QString(),   
           MenuEntryKind  ty = Entry_None, 
                     int  tg = -1,
           const QString & in = QString())
      :title (ti), tipo (ty), tag (tg), iconName (in)
    {}
    QString        title;
    MenuEntryKind  tipo;
    int            tag;
    QString        iconName;
  };

  QList <Entry>  items;
  QString        theTitle;
 
};

} // namespace

#endif
