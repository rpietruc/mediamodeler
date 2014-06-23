#ifndef MEDIA_IMAGECURVATUREFLOWTRANSFORM_H
#define MEDIA_IMAGECURVATUREFLOWTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imagecurvatureflow_EXPORTS)
#  define IMAGECURVATUREFLOWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGECURVATUREFLOWSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGECURVATUREFLOWSHARED_EXPORT ImageCurvatureFlowTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageCurvatureFlowTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    FloatImageFrame mSrcFrame;
    FloatImageFrame mImageFrame;
    };

class IMAGECURVATUREFLOWSHARED_EXPORT ImageCurvatureFlowTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageCurvatureFlowTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageCurvatureFlowTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGECURVATUREFLOWTRANSFORM_H
