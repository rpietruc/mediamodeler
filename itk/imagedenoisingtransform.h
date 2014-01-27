#ifndef MEDIA_IMAGEDENOISINGTRANSFORM_H
#define MEDIA_IMAGEDENOISINGTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imagedenoising_EXPORTS)
#  define IMAGEDENOISINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEDENOISINGSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

/*! Curvature flow image filter for producing homogeneous regions
 */

class IMAGEDENOISINGSHARED_EXPORT ImageDenoisingTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageDenoisingTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    GrayImageFrame mImageFrame;
    };

class IMAGEDENOISINGSHARED_EXPORT ImageDenoisingTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageDenoisingTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageDenoisingTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEDENOISINGTRANSFORM_H
