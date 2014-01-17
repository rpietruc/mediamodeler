#ifndef MEDIA_IMAGESMOOTHINGTRANSFORM_H
#define MEDIA_IMAGESMOOTHINGTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(IMAGESMOOTHING_LIBRARY)
#  define IMAGESMOOTHINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGESMOOTHINGSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

/*! Curvature flow image filter for producing homogeneous regions
 */

class IMAGESMOOTHINGSHARED_EXPORT ImageSmoothingTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageSmoothingTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

    ParamList getParams();

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    GrayImageFrame mImageFrame;

    double mRangeSigma;
    double mDomainSigma;
    const QVector<QString> mDescriptions;
    };

class IMAGESMOOTHINGSHARED_EXPORT ImageSmoothingTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageSmoothingTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageSmoothingTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGESMOOTHINGTRANSFORM_H
