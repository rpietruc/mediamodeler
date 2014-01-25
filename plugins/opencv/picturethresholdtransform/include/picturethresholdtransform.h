#ifndef MEDIA_PICTURETHRESHOLDTRANSFORM_H
#define MEDIA_PICTURETHRESHOLDTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(PICTURETHRESHOLD_LIBRARY)
#  define PICTURETHRESHOLDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURETHRESHOLDSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

/*! threshold the colors of the image
 */
class PICTURETHRESHOLDSHARED_EXPORT PictureThresholdTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureThresholdTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

private:
    IplImageFrame mSrcFrame;
    IplImageFrame mPictureFrame;
    };

class PICTURETHRESHOLDSHARED_EXPORT PictureThresholdTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureThresholdTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureThresholdTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTURETHRESHOLDTRANSFORM_H
