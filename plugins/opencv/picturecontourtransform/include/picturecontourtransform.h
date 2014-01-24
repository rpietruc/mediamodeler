#ifndef MEDIA_PICTURECONTOURTRANSFORM_H
#define MEDIA_PICTURECONTOURTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(PICTURECONTOURTRANSFORM_LIBRARY)
#  define PICTURECONTOURTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURECONTOURTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTURECONTOURTRANSFORMSHARED_EXPORT PictureContourTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureContourTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return mPointsFrameSet.size(); }
    const FrameBase *getFrame(int aIndex) const { return &mPointsFrameSet.at(aIndex); }

    ParamList getParams() const;

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();

    void setMethod(int aMethod);

private:
    IplImageFrame mSrcFrame;
    IplImageFrame mGrayFrame;
    QVector<PointsFrame> mPointsFrameSet;
    int mMethod;
    };

class PICTURECONTOURTRANSFORMSHARED_EXPORT PictureContourTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureContourTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureContourTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTURECONTOURTRANSFORM_H
