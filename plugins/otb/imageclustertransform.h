#ifndef MEDIA_IMAGECLUSTERTRANSFORM_H
#define MEDIA_IMAGECLUSTERTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imagecluster_EXPORTS)
#  define IMAGECLUSTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGECLUSTERSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGECLUSTERSHARED_EXPORT ImageClusterTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageClusterTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    GrayImageFrame mImageFrame;
    };

class IMAGECLUSTERSHARED_EXPORT ImageClusterTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageClusterTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageClusterTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGECLUSTERTRANSFORM_H
