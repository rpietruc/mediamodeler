#ifndef MEDIA_ACOUSTICTUBETRANSFORM_H
#define MEDIA_ACOUSTICTUBETRANSFORM_H

#include "elementbase.h"
#include "matrixframe.h"
#include <QtCore/qglobal.h>
#include <filterconverter.h>

#if defined(ACOUSTICTUBETRANSFORM_EXPORTS)
#  define ACOUSTICTUBETRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACOUSTICTUBETRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class ACOUSTICTUBETRANSFORMSHARED_EXPORT AcousticTubeTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit AcousticTubeTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mAreasFrame; }

private:
    void process();

    acoustics::CFilterConverter<float> *mFilterConverter;
    MatrixFrame mAreasFrame;
};

class ACOUSTICTUBETRANSFORMSHARED_EXPORT AcousticTubeTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    inline const char* getElementName() const { return "AcousticTubeTransform"; }
    inline ElementBase* createElement(const QString &aObjectName) { return new AcousticTubeTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_ACOUSTICTUBETRANSFORM_H
