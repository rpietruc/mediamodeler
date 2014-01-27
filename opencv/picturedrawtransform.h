#ifndef MEDIA_PICTUREDRAWTRANSFORM_H
#define MEDIA_PICTUREDRAWTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(picturedrawtransform_EXPORTS)
#  define PICTUREDRAWTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREDRAWTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREDRAWTRANSFORMSHARED_EXPORT PictureDrawTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureDrawTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

private:
    IplImageFrame mPictureFrame;
    };

class PICTUREDRAWTRANSFORMSHARED_EXPORT PictureDrawTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureDrawTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureDrawTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREDRAWTRANSFORM_H
