#ifndef MEDIA_PICTUREAPPROXIMATIONTRANSFORM_H
#define MEDIA_PICTUREAPPROXIMATIONTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(PICTUREAPPROXIMATIONTRANSFORM_LIBRARY)
#  define PICTUREAPPROXIMATIONTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREAPPROXIMATIONTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREAPPROXIMATIONTRANSFORMSHARED_EXPORT PictureApproximationTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureApproximationTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    //int getFramesNo() const { return mPointsFrameSet.size(); }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }
    //const FrameBase *getFrame(int aIndex) const { return &mPointsFrameSet.at(aIndex); }

    ParamList getParams();

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();

private:
    IplImageFrame mPictureFrame;
    //QVector<PointsFrame> mPointsFrameSet;

    double mMinArea;
    double mAccuracy;
    };

class PICTUREAPPROXIMATIONTRANSFORMSHARED_EXPORT PictureApproximationTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureApproximationTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureApproximationTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREAPPROXIMATIONTRANSFORM_H
