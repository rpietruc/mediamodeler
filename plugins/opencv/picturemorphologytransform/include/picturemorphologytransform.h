#ifndef MEDIA_PICTUREMORPHOLOGYTRANSFORM_H
#define MEDIA_PICTUREMORPHOLOGYTRANSFORM_H

#include "pictureframes.h"
#include "elementbase.h"
#include <opencv/cv.h>
#include <string>
#include <QtCore/qglobal.h>

#if defined(PICTUREMORPHOLOGY_LIBRARY)
#  define PICTUREMORPHOLOGYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREMORPHOLOGYSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREMORPHOLOGYSHARED_EXPORT PictureMorphologyTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureMorphologyTransform(ElementFactory *aFactory, const QString &aObjectName);
    ~PictureMorphologyTransform();

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

private:
    void process();

private:
    IplImageFrame mSrcFrame;
    IplImageFrame mPictureFrame;
    IplImageFrame mTempFrame;
    IplConvKernel* mStructuringElement;
    };

class PICTUREMORPHOLOGYSHARED_EXPORT PictureMorphologyTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureMorphologyTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureMorphologyTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREMORPHOLOGYTRANSFORM_H
