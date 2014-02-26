#ifndef MEDIA_IMAGEANDTRANSFORM_H
#define MEDIA_IMAGEANDTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imageandtransform_EXPORTS)
#  define IMAGEANDTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEANDTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGEANDTRANSFORMSHARED_EXPORT ImageAndTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageAndTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    QVector<GrayImageFrame> mSrcFrames;
    GrayImageFrame mImageFrame;
    };

class IMAGEANDTRANSFORMSHARED_EXPORT ImageAndTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageAndTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageAndTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEANDTRANSFORM_H
