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
  Entry_Navigate = 3
};

class MenuModel : public QAbstractListModel
{
  Q_OBJECT
public:


  MenuModel (QObject * parent=0);

  int rowCount (const QModelIndex & parent = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  void addSubmenu (const QString & title,
                   const QString & desktop,
                   int       menuTag,
                   const QString & iconName);
  void addAppLink (const QString & title,
                   const QString & desktop,
                   int       appTag,
                   const QString & iconName);
  void addNavigate (const QString & title, 
                   int       tag);

  QString title () const;
  void    setTitle (const QString & title);

  void fakeReset ();

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
           const QString & d = QString(), 
           MenuEntryKind  ty = Entry_None, 
                     int  tg = -1,
           const QString & in = QString())
      :title (ti), desktop (d), tipo (ty), tag (tg), iconName (in)
    {}
    QString    title;
    QString    desktop;
    MenuEntryKind  tipo;
    int        tag;
    QString    iconName;
  };

  QList <Entry>  items;
  QString        theTitle;
 
};

} // namespace

#endif
