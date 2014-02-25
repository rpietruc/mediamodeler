#ifndef MEDIA_IMAGETHINNINGTRANSFORM_H
#define MEDIA_IMAGETHINNINGTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imagethinning_EXPORTS)
#  define IMAGETHINNINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGETHINNINGSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

/*! Curvature flow image filter for producing homogeneous regions
 */

class IMAGETHINNINGSHARED_EXPORT ImageThinningTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageThinningTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    GrayImageFrame mImageFrame;
    };

class IMAGETHINNINGSHARED_EXPORT ImageThinningTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageThinningTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageThinningTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGETHINNINGTRANSFORM_H
