#include "geuzen-imagepro.h"
#include <iostream>

namespace geuzen
{
ImagePro::ImagePro ()
  :QDeclarativeImageProvider (QDeclarativeImageProvider::Pixmap)
{
}

QPixmap 
ImagePro::requestPixmap(const QString &id, 
                        QSize *size, 
                        const QSize &requestedSize)
{
  std::cerr << __PRETTY_FUNCTION__ << " " << id.toStdString() 
            << " " << requestedSize.width() << "x" << requestedSize.height()
            << std::endl;
  QPixmap pixIcon;
  QSize getSize (256,256);
  if (requestedSize.isValid()) {
    getSize = requestedSize;
  }
  if (icons.contains (id)) {
    std::cerr << __PRETTY_FUNCTION__ << " have image " << id.toStdString()
              << std::endl;
    pixIcon = icons[id].pixmap (getSize);
    if (size) {
      *size = pixIcon.size();
    }
  } else {
    std::cerr << __PRETTY_FUNCTION__ << " NO image " << id.toStdString()
              << std::endl;
    if (size) {
      *size = QSize (0,0);
    }
  }
  return pixIcon;
}

void
ImagePro::addIcon (const QString & id, const QIcon & icon)
{
  std::cerr << __PRETTY_FUNCTION__ << " " << id.toStdString() << std::endl;
  icons[id] = icon;
}

} // namespace
