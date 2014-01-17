#ifndef MEDIA_IMAGEGENERATORSOURCE_H
#define MEDIA_IMAGEGENERATORSOURCE_H

#include "imageframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(IMAGEGENERATOR_LIBRARY)
#  define IMAGEGENERATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEGENERATORSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

/*! Curvature flow image filter for producing homogeneous regions
 */

class IMAGEGENERATORSHARED_EXPORT ImageGeneratorSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit ImageGeneratorSource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

    ParamList getParams();

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();

    bool drawRect(double x, double y);
    bool drawEllipse(double x, double y);

private:
    ColorImageFrame mImageFrame;
    ColorImageFrame::ImageType::SizeType mSize;
    const QVector<QString> mDescriptions;
    };

class IMAGEGENERATORSHARED_EXPORT ImageGeneratorSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ImageGeneratorSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new ImageGeneratorSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_IMAGEGENERATORSOURCE_H
