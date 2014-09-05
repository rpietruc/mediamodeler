#ifndef PICTUREBINARYTRANSFORM_H
#define PICTUREBINARYTRANSFORM_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(picturebinarytransform_EXPORTS)
#  define PICTUREBINARYTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREBINARYTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREBINARYTRANSFORMSHARED_EXPORT PictureBinaryTransform : public ElementBase
    {
    Q_OBJECT

public:
    PictureBinaryTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

    static CvMat* createMatFromImage(const IplImage* img);
    static void imageFromMat(IplImage* img, const CvMat *mtx);
    static void cvKMeansTest(const CvMat *pic, CvMat *cls, int dim);

    PictureGrayFrame mPictureFrame;
    };

class PICTUREBINARYTRANSFORMSHARED_EXPORT PictureBinaryTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureBinaryTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureBinaryTransform(this, aObjectName); }
    };

} // namespace media

#endif // PICTUREBINARYTRANSFORM_H
