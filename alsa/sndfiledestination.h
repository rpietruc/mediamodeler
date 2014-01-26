#ifndef MEDIA_SNDFILEDESTINATION_H
#define MEDIA_SNDFILEDESTINATION_H

#include "elementbase.h"
#include <QString>
#include <sndfile.h>
#include <QtCore/qglobal.h>
#include "alsaframe.h"

#if defined(sndfiledestination_EXPORTS)
#  define SNDFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SNDFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class SNDFILEDESTINATIONSHARED_EXPORT SndFileDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit SndFileDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~SndFileDestination();

private slots:
    void open();
    void close();
    void write();

private:
    void process();

private:
    SNDFILE *mSoundFile;
    AlsaFrame mAlsaFrame;
    };

class SndFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SndFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new SndFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_SNDFILEDESTINATION_H
