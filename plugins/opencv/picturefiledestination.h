#ifndef MEDIA_PictureFileDestination_H
#define MEDIA_PictureFileDestination_H

#include "filedestination.h"

#if defined(picturefiledestination_EXPORTS)
#  define PICTUREFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREFILEDESTINATIONSHARED_EXPORT PictureFileDestination : public FileDestination
    {
    Q_OBJECT

public:
    explicit PictureFileDestination(ElementFactory *aFactory, const QString &aObjectName);

private:
    void process();
    };

class PictureFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PictureFileDestination_H
