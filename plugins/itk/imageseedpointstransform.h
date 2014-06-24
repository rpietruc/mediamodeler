#ifndef MEDIA_IMAGESEEDPOINTSTRANSFORM_H
#define MEDIA_IMAGESEEDPOINTSTRANSFORM_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(imageseedpoints_EXPORTS)
#  define IMAGESEEDPOINTSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGESEEDPOINTSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class IMAGESEEDPOINTSSHARED_EXPORT ImageSeedPointsTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageSeedPointsTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPointsFrame; }

private:
    void process();

private:
    GrayImageFrame mSrcFrame;
    PointsFrame mPointsFrame;
    };

class IMAGESEEDPOINTSSHARED_EXPORT ImageSeedPointsTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageSeedPointsTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageSeedPointsTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGESEEDPOINTSTRANSFORM_H
