#ifndef MEDIA_POTRACEGENERATORSOURCE_H
#define MEDIA_POTRACEGENERATORSOURCE_H

#include "potraceframes.h"
#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(potracegenerator_EXPORTS)
#  define POTRACEGENERATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define POTRACEGENERATORSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class POTRACEGENERATORSHARED_EXPORT PotraceGeneratorSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit PotraceGeneratorSource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mImageFrame; }

private:
    void process();

private:
    PotraceImageFrame mImageFrame;
    };

class POTRACEGENERATORSHARED_EXPORT PotraceGeneratorSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PotraceGeneratorSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new PotraceGeneratorSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_POTRACEGENERATORSOURCE_H
