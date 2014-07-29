#ifndef MEDIA_IMAGEFILEDESTINATION_H
#define MEDIA_IMAGEFILEDESTINATION_H

#include "filedestination.h"

#if defined(imagedenoising_EXPORTS)
#  define IMAGEEXTRACTROADSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEEXTRACTROADSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGEEXTRACTROADSHARED_EXPORT ImageFileDestination : public FileDestination
    {
    Q_OBJECT

public:
    explicit ImageFileDestination(ElementFactory *aFactory, const QString &aObjectName);

private:
    void process();
    };

class IMAGEEXTRACTROADSHARED_EXPORT ImageFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEFILEDESTINATION_H
