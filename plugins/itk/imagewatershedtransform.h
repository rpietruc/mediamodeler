#ifndef MEDIA_IMAGEWATERSHEDTRANSFORM_H
#define MEDIA_IMAGEWATERSHEDTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imagewatershed_EXPORTS)
#  define IMAGEWATERSHEDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEWATERSHEDSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGEWATERSHEDSHARED_EXPORT ImageWatershedTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageWatershedTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    ColorImageFrame mImageFrame;
    };

class IMAGEWATERSHEDSHARED_EXPORT ImageWatershedTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageWatershedTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageWatershedTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEWATERSHEDTRANSFORM_H
