#ifndef MEDIA_PICTUREEDGEDETECTIONTRANSFORM_H
#define MEDIA_PICTUREEDGEDETECTIONTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(pictureedgedetectiontransform_EXPORTS)
#  define PICTUREEDGEDETECTIONTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREEDGEDETECTIONTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREEDGEDETECTIONTRANSFORMSHARED_EXPORT PictureEdgeDetectionTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureEdgeDetectionTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mGrayImg; }

private:
    void process();

private:
    PictureGrayFrame mGrayImg;
    };

class PICTUREEDGEDETECTIONTRANSFORMSHARED_EXPORT PictureEdgeDetectionTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureEdgeDetectionTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureEdgeDetectionTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREEDGEDETECTIONTRANSFORM_H
