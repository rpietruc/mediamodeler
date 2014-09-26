#ifndef MEDIA_VectorFileSource_H
#define MEDIA_VectorFileSource_H

#include "filelistsource.h"
#include "pictureframes.h"

#if defined(vectorfiledestination_EXPORTS)
#  define VECTORFILESOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define VECTORFILESOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class VECTORFILESOURCESHARED_EXPORT VectorFileSource : public FileListSource
    {
    Q_OBJECT

public:
    explicit VectorFileSource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mRGBImg; }

private:
    void process();

private:
    PictureRGBFrame mRGBImg;
    };

class VectorFileSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "VectorFileSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new VectorFileSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_VectorFileSource_H
