#ifndef MEDIA_IMAGEREGIONGROWINGTRANSFORM_H
#define MEDIA_IMAGEREGIONGROWINGTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imageregiongrowing_EXPORTS)
#  define IMAGEREGIONGROWINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEREGIONGROWINGSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGEREGIONGROWINGSHARED_EXPORT ImageRegionGrowingTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageRegionGrowingTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    FloatImageFrame mSrcFrame;
    GrayImageFrame mImageFrame;
    };

class IMAGEREGIONGROWINGSHARED_EXPORT ImageRegionGrowingTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageRegionGrowingTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageRegionGrowingTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEREGIONGROWINGTRANSFORM_H
