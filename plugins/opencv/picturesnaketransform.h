#ifndef PICTURESNAKETRANSFORM_H
#define PICTURESNAKETRANSFORM_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(picturesnaketransform_LIBRARY)
#  define PICTURESNAKETRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURESNAKETRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTURESNAKETRANSFORMSHARED_EXPORT PictureSnakeTransform : public ElementBase
    {
    Q_OBJECT

public:
    PictureSnakeTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame() const { return &mPointsFrame; }

private:
    void process();

private:
    PointsFrame mPointsFrame;
    };

class PICTURESNAKETRANSFORMSHARED_EXPORT PictureSnakeTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureSnakeTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureSnakeTransform(this, aObjectName); }
    };

} // namespace media

#endif // PICTURESNAKETRANSFORM_H
