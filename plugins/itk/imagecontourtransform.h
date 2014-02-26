#ifndef MEDIA_IMAGECONTOURTRANSFORM_H
#define MEDIA_IMAGECONTOURTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imagedenoising_EXPORTS)
#  define IMAGECONTOURSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGECONTOURSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGECONTOURSHARED_EXPORT ImageContourTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageContourTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return mPointsFrameSet.size(); }
    const FrameBase *getFrame(int aIndex) const { return &mPointsFrameSet.at(aIndex); }

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    QVector<PointsFrame> mPointsFrameSet;
    };

class IMAGECONTOURSHARED_EXPORT ImageContourTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageContourTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageContourTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGECONTOURTRANSFORM_H
