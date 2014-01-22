#ifndef MEDIA_IMAGEADDTRANSFORM_H
#define MEDIA_IMAGEADDTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(IMAGEADDTRANSFORM_LIBRARY)
#  define IMAGEADDTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEADDTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

/*! Curvature flow image filter for producing homogeneous regions
 */

class IMAGEADDTRANSFORMSHARED_EXPORT ImageAddTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageAddTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    QVector<GrayImageFrame> mSrcFrames;
    GrayImageFrame mImageFrame;
    };

class IMAGEADDTRANSFORMSHARED_EXPORT ImageAddTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageAddTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageAddTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEADDTRANSFORM_H
