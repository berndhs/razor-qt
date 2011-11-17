#ifndef GEUZEN_IMAGE_PRO_H
#define GEUZEN_IMAGE_PRO_H

#include <QtDeclarative/QDeclarativeImageProvider>
#include <QString>
#include <QIcon>
#include <QMap>
#include <QPixmap>

namespace geuzen
{
class ImagePro : public QDeclarativeImageProvider
{
public:
  ImagePro ();

  QPixmap requestPixmap(const QString &id, 
                        QSize *size, 
                        const QSize &requestedSize);

  void addIcon (const QString & id,
                 const QIcon & icon);

private:

  QMap <QString, QIcon>  icons;
};

} // namespace


#endif
