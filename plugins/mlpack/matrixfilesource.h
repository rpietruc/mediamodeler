#ifndef MEDIA_MATRIXFILESOURCE_H
#define MEDIA_MATRIXFILESOURCE_H

#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(matrixfile_EXPORTS)
#  define MATRIXFILESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MATRIXFILESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class MATRIXFILESHARED_EXPORT MatrixFileSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit MatrixFileSource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return mMatrixFrame; }

private:
    void process();

private:
    FrameBase *mMatrixFrame;
    };

class MATRIXFILESHARED_EXPORT MatrixFileSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "MatrixFileSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new MatrixFileSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_MATRIXFILESOURCE_H
