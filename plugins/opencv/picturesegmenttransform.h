#ifndef PICTURESEGMENTTRANSFORM_H
#define PICTURESEGMENTTRANSFORM_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(picturesegmenttransform_EXPORTS)
#  define PICTURESEGMENTTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURESEGMENTTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTURESEGMENTTRANSFORMSHARED_EXPORT PictureSegmentTransform : public ElementBase
    {
    Q_OBJECT

public:
    PictureSegmentTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

    static CvMat* createMatFromImage(const IplImage* img);
    static void imageFromMat(IplImage* img, const CvMat *mtx);
    static void cvKMeansTest(const CvMat *pic, CvMat *cls, int dim);

    PictureGrayFrame mPictureFrame;
    };

class PICTURESEGMENTTRANSFORMSHARED_EXPORT PictureSegmentTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureSegmentTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureSegmentTransform(this, aObjectName); }
    };

} // namespace media

#endif // PICTURESEGMENTTRANSFORM_H
