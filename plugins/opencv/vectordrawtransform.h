#ifndef MEDIA_ELLIPSEDRAWTRANSFORM_H
#define MEDIA_ELLIPSEDRAWTRANSFORM_H

#include <pictureframes.h>
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(ellipsedrawtransform_EXPORTS)
#  define ELLIPSEDRAWTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ELLIPSEDRAWTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class ELLIPSEDRAWTRANSFORMSHARED_EXPORT EllipseDrawTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit EllipseDrawTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

private:
    IplImageFrame mPictureFrame;
    };

class ELLIPSEDRAWTRANSFORMSHARED_EXPORT EllipseDrawTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "EllipseDrawTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new EllipseDrawTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_ELLIPSEDRAWTRANSFORM_H
