#ifndef MEDIA_VECTORDRAWTRANSFORM_H
#define MEDIA_VECTORDRAWTRANSFORM_H

#include <pictureframes.h>
#include "elementbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(vectordrawtransform_EXPORTS)
#  define VECTORDRAWTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VECTORDRAWTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class VECTORDRAWTRANSFORMSHARED_EXPORT VectorDrawTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit VectorDrawTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

private:
    PictureRGBFrame mPictureFrame;
    };

class VECTORDRAWTRANSFORMSHARED_EXPORT VectorDrawTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "VectorDrawTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new VectorDrawTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_VECTORDRAWTRANSFORM_H
