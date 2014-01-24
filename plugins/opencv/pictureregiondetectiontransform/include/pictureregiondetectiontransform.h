#ifndef MEDIA_PICTUREREGIONDETECTIONTRANSFORM_H
#define MEDIA_PICTUREREGIONDETECTIONTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <string>
#include <QtCore/qglobal.h>

#if defined(PICTUREREGIONDETECTION_LIBRARY)
#  define PICTUREREGIONDETECTIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREREGIONDETECTIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREREGIONDETECTIONSHARED_EXPORT PictureRegionDetectionTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureRegionDetectionTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

    ParamList getParams() const;

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();
    void setCascadeClassifier(const QString &aFileName);

    IplImageFrame mPictureFrame;
    cv::CascadeClassifier mCascadeClassifier;
    std::vector<cv::Rect> mRectVector;
    };

class PICTUREREGIONDETECTIONSHARED_EXPORT PictureRegionDetectionTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureRegionDetectionTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureRegionDetectionTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREREGIONDETECTIONTRANSFORM_H
