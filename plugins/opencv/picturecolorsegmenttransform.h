#ifndef PICTURECOLORSEGMENTTRANSFORM_H
#define PICTURECOLORSEGMENTTRANSFORM_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(picturesegmenttransform_EXPORTS)
#  define PICTURECOLORSEGMENTTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURECOLORSEGMENTTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTURECOLORSEGMENTTRANSFORMSHARED_EXPORT PictureColorSegmentTransform : public ElementBase
    {
    Q_OBJECT

public:
    PictureColorSegmentTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

    PictureGrayFrame mPictureFrame;
    };

class PICTURECOLORSEGMENTTRANSFORMSHARED_EXPORT PictureColorSegmentTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureColorSegmentTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureColorSegmentTransform(this, aObjectName); }
    };

} // namespace media

#endif // PICTURECOLORSEGMENTTRANSFORM_H
