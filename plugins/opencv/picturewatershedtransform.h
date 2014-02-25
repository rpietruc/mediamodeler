#ifndef MEDIA_PICTUREWATERSHEDTRANSFORM_H
#define MEDIA_PICTUREWATERSHEDTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <string>
#include <QtCore/qglobal.h>

#if defined(picturewatershedtransform_EXPORTS)
#  define PICTUREWATERSHEDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREWATERSHEDSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREWATERSHEDSHARED_EXPORT PictureWatershedTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureWatershedTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

    IplImageFrame mSrcFrame;
    IplImageFrame mPictureFrame;
    };

class PICTUREWATERSHEDSHARED_EXPORT PictureWatershedTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureWatershedTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureWatershedTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREWATERSHEDTRANSFORM_H
