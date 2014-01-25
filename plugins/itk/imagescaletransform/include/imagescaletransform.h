#ifndef MEDIA_IMAGESCALETRANSFORM_H
#define MEDIA_IMAGESCALETRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(IMAGESCALE_LIBRARY)
#  define IMAGESCALESHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGESCALESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGESCALESHARED_EXPORT ImageScaleTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageScaleTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    ColorImageFrame mSrcFrame;
    ColorImageFrame mImageFrame;
    };

class IMAGESCALESHARED_EXPORT ImageScaleTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageScaleTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageScaleTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGESCALETRANSFORM_H
