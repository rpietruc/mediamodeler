#ifndef MEDIA_IMAGEVECTORSOURCE_H
#define MEDIA_IMAGEVECTORSOURCE_H

#include "filelistsource.h"
#include "otbframes.h"
#include <QtCore/qglobal.h>

#if defined(imagevector_EXPORTS)
#  define IMAGEVECTORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEVECTORSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGEVECTORSHARED_EXPORT ImageVectorSource : public FileListSource
    {
    Q_OBJECT

public:
    explicit ImageVectorSource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int aIndex) const { return &mImageFrame; }

private:
    void process();

private:
    VectorOtbFrame mImageFrame;
    };

class IMAGEVECTORSHARED_EXPORT ImageVectorSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageVectorSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageVectorSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEVECTORSOURCE_H
