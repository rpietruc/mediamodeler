#ifndef MEDIA_POTRACETRACETRANSFORM_H
#define MEDIA_POTRACETRACETRANSFORM_H

#include "elementbase.h"
#include "potraceframes.h"
#include <QtCore/qglobal.h>

#if defined(potracetrace_EXPORTS)
#  define POTRACETRACESHARED_EXPORT Q_DECL_EXPORT
#else
#  define POTRACETRACESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class POTRACETRACESHARED_EXPORT PotraceTraceTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit PotraceTraceTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPathFrame; }

private:
    void process();

    friend void printProgress(double progress, void *privdata);
    void logProgress(double progress);

private:
    PotracePathFrame mPathFrame;
    };

class POTRACETRACESHARED_EXPORT PotraceTraceTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PotraceTraceTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new PotraceTraceTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_POTRACETRACETRANSFORM_H
