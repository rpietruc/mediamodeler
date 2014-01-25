#ifndef PICTURECLUSTERTRANSFORM_H
#define PICTURECLUSTERTRANSFORM_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(PICTURECLUSTERTRANSFORM_LIBRARY)
#  define PICTURECLUSTERTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURECLUSTERTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTURECLUSTERTRANSFORMSHARED_EXPORT PictureClusterTransform : public ElementBase
    {
    Q_OBJECT

public:
    PictureClusterTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

//    ParamList getParams() const;

protected slots:
//    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();

    static CvMat* createMatFromImage(const IplImage* img);
    static void imageFromMat(IplImage* img, const CvMat *mtx);
    static void cvKMeansTest(const CvMat *pic, CvMat *cls, int dim);

    IplImageFrame mSrcFrame;
    IplImageFrame mPictureFrame;
    int mClustersNo;
    };

class PICTURECLUSTERTRANSFORMSHARED_EXPORT PictureClusterTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureClusterTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureClusterTransform(this, aObjectName); }
    };

} // namespace media

#endif // PICTURECLUSTERTRANSFORM_H
