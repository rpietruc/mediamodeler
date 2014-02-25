#ifndef MEDIA_SNDFILESOURCE_H
#define MEDIA_SNDFILESOURCE_H

#include "elementbase.h"
#include <QStringList>
#include <sndfile.h>
#include "alsaframe.h"
#include <QtCore/qglobal.h>

#if defined(sndfilesource_EXPORTS)
#  define SNDFILESOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SNDFILESOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class SNDFILESOURCESHARED_EXPORT SndFileSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit SndFileSource(ElementFactory *aFactory, const QString &aObjectName);
     ~SndFileSource();

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mAlsaFrame; }

    bool event(QEvent *aEvent);

private:
    void process();
    bool openNextFile();

    AlsaFrame mAlsaFrame;
    QStringList mPathList;
    int mNextFileIndex;
    SNDFILE *mSoundFile;
    };

class SndFileSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SndFileSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new SndFileSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_SNDFILESOURCE_H
